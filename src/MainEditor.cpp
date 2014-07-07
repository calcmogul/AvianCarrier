//=============================================================================
//File Name: MainEditor.cpp
//Description: Handles main editor and window
//Author: Tyler Veness
//=============================================================================

//TODO: Implement text selection with highlighting (Ctrl + A flag will be special case of this) -> send this to copy function

//TODO: Progress Bars (not needed yet)
//TODO: Save Dialog
//TODO: Ability to tab through buttons (button outline will be blue)

//FIXME: Adjust restore feature so that it will open files on server upon restart
//FIXME: Files won't save locally unless all of its parent directories already exist

#include <iostream>//FIXME

#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Sleep.hpp>

#include "Base.h"
#include "GUI/AutoResize.h"
#include "GUI/Tab.h"
#include "GUI/Button.h"
#include "GUI/DropDown.h"
#include "GUI/Toolbar.h"
#include "GUI/SplashScreen.h"
#include "GUI/StatusBar.h"
#include "GUI/ScrollBar.h"
#include "GUI/InputBox.h"
#include "GUI/ChatWindow.h"
#include "GUI/Editor.h"
#include "GUI/Cursor.hpp"
#include "GUI/MenuSelect.h"
#include "DirList.h"

void closeProgram();
void openFile();
void copyText();
void pasteText();
void buildEXE();
void displayAbout();
int sysRun( std::string programAndDir , std::string args );

sf::Thread openThread( openFile );
sf::Thread buildThread( buildEXE );
sf::Thread aboutThread( displayAbout );

sf::RenderWindow mainWin;
HINSTANCE globalInstance; //FIXME win32 HINSTANCE
volatile bool CLOSE_THREADS = false;

Base temp; //loads all files for base class

StatusBar statusBar;

std::string searchDir;

LRESULT CALLBACK OnEvent( HWND Handle , UINT Message , WPARAM WParam , LPARAM LParam );

Button file( "File" , "" , "" , -1 , 30 , []{} );
Button newFile( "New" , "Ctrl + N" , "Control N" , -1 , 40 , []{ Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) ); } );
Button open( "Open" , "Ctrl + O" , "Control O" , -1 , 40 , []{ openThread.launch(); } );
Button save( "Save Local" , "Ctrl + S" , "Control S" , -1 , 40 , []{ Tab::current->saveLocal(); } , false );
Button exitButton( "Exit" , " " , "" , -1 , 40 , []{ closeProgram(); } );

Button edit( "Edit" , "" , "" , -1 , 30 , []{} );
Button undo( "Undo" , "Ctrl + Z" , "Control Z" , -1 , 40 , []{} , false );
Button redo( "Redo" , "Ctrl + Y" , "Control Y" , -1 , 40 , []{} , false );
Button cut( "Cut" , "Ctrl + X" , "Control X" , -1 , 40 , []{} , false );
Button copy( "Copy" , "Ctrl + C" , "Control C" , -1 , 40 , []{ copyText(); } , false );
Button paste( "Paste" , "Ctrl + V" , "Control V" , -1 , 40 , []{ pasteText(); } );

Button build( "Build" , "" , "" , -1 , 30 , []{} );
Button buildDebug( "Debug" , "Ctrl + B" , "Control B" , -1 , 40 , []{ buildThread.launch(); } );
Button buildRelease( "Release" , " " , "" , -1 , 40 , []{} );

Button collaborate( "Collaborate" , "" , "" , -1 , 30 , []{} );
Button chat( "Chat" , "Ctrl + T" , "Control T" , -1 , 40 , []{ ChatWindow::toggleChat(); } , false );

Button options( "Options" , "" , "" , -1 , 30 , []{} );
Button settings( "Settings..." , " " , "" , -1 , 40 , []{} , false );
Button about( "About" , " " , "" , -1 , 40 , []{ aboutThread.launch(); } );

DropDown fileMenu( 0 , 4 , { &file , &newFile , &open , &save , &exitButton } );
DropDown editMenu( 0 , 4 , { &edit , &undo , &redo , &cut , &copy , &paste } );
DropDown buildMenu( 0 , 4 , { &build , &buildDebug , &buildRelease } );
DropDown collaborateMenu( 0 , 4 , { &collaborate , &chat } );
DropDown optionMenu( 0 , 4 , { &options , &settings , &about } );

ToolBar mainTools( 0.f , 0.f , mainWin.getSize().x , 24.f , { &fileMenu , &editMenu , &buildMenu , &collaborateMenu , &optionMenu } );

Editor editBackground;

INT WINAPI WinMain( HINSTANCE Instance , HINSTANCE , LPSTR , INT ) {
	TextReceiver::textInterfaces.push_back( &editBackground ); // add editor back to textReceiver array because it was mysteriously removed after entering WinMain...

	std::vector<sf::Color> temp = { sf::Color( 45 , 45 , 45 ) , sf::Color( 100 , 100 , 100 ) , sf::Color( 100 , 100 , 100 ) , sf::Color( 45 , 45 , 45 ) };
	ChatWindow::createInstance( sf::Vector2f( 200.f , 600.f ) , temp );
	ChatWindow* chatWindow = ChatWindow::getInstance();

	freopen( "error.log" , "w+" , stderr );

	// Define a class for our main window
	WNDCLASS WindowClass;
	WindowClass.style         = 0;
	WindowClass.lpfnWndProc   = &OnEvent;
	WindowClass.cbClsExtra    = 0;
	WindowClass.cbWndExtra    = 0;
	WindowClass.hInstance     = Instance;
	WindowClass.hIcon         = NULL;
	WindowClass.hCursor       = NULL;
	WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
	WindowClass.lpszMenuName  = NULL;
	WindowClass.lpszClassName = mainClassName;
	RegisterClass(&WindowClass);

	globalInstance = Instance;

	sf::Window centerWin;
	centerWin.setVisible( false );
	centerWin.create( sf::VideoMode( 0 , 0 ) , "" , sf::Style::None );

	HWND Window = CreateWindow( mainClassName , "AvianCarrier" , WS_SYSMENU | WS_VISIBLE | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME , ( GetSystemMetrics(SM_CXSCREEN) - 800 ) / 2 , ( GetSystemMetrics(SM_CYSCREEN) - 600 ) / 2 , 800 , 600 , NULL , NULL , Instance , NULL );

	centerWin.close();
	mainWin.create( Window );

	sf::Image appIcon;
	if ( !appIcon.loadFromFile( "Resource/Pigeot.png" ) ) {
		return EXIT_FAILURE;
	}
	appIcon.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );

	mainWin.setIcon( appIcon.getSize().x , appIcon.getSize().y , appIcon.getPixelsPtr() );

	searchDir = "Documents";

	File::bindSockets(); // bind sockets to client's ports so server communications will succeed

	/* ===== Restore previous session ===== */
	std::ifstream sessionStore( "Config/session.txt" );
	if ( sessionStore.is_open() ) {
		std::string temp;
		unsigned int currentTabPos;

		if ( !sessionStore.eof() ) { // if there are saved tabs to open
			sessionStore >> currentTabPos; // extract position of current tab, to be set later

			if ( !sessionStore.eof() ) { // if number extracted for current tab isn't bogus
				while ( !sessionStore.eof() ) { // while there are still saved tabs to open
					std::getline( sessionStore , temp );

					if ( temp != "" )
						Tab::newTab( sf::IpAddress( temp.substr( temp.find( "@" ) + 1 ) ) , temp.substr( 0 , temp.find( "@" ) ) );
				}

				// Semaphores aren't needed here because no threads destruct pointers
				Tab::current = Tab::tabsOpen[currentTabPos]; // restore which tab is current from last session
			}
		}
	}
	else {
		std::cerr << "'session.txt' failed to open\n";
	}
	/* ==================================== */

	sf::Event event;
	sf::Cursor normalCursor( sf::Cursor::Normal );
	sf::Cursor textCursor( sf::Cursor::Text );

	Button* currentButtonHovered = NULL;

	mainTools.setVisible( true );

	Tab::draw( mainWin );

	MSG Message; // FIXME win32 MSG

	PostMessage( Window , WM_SIZE , 0 , 0 ); // Send resize message to window so all GUI elements draw at least once

	while ( mainWin.isOpen() ) {
		// FIXME win32 message handler
		if ( PeekMessage( &Message , NULL , 0 , 0 , PM_REMOVE ) ) {
			// If a message was waiting in the message queue, process it
			TranslateMessage( &Message );
			DispatchMessage( &Message );
		}
		else {
			while ( mainWin.pollEvent( event ) ) {
				if ( event.type == sf::Event::Closed ) {
					// wait for all threads to close before closing the window so it is obvious if they don't respond
					CLOSE_THREADS = true;
					openThread.wait();
					aboutThread.wait();

					delete ChatWindow::getInstance();
					chatWindow = NULL;

					/* ===== Save current session ===== */
					std::ofstream sessionStore( "Config/session.txt" , std::ios_base::trunc );
					if ( sessionStore.is_open() ) {
						if ( Tab::tabsOpen.size() > 0 ) { // if there is a current tab (which means there are also tabs to save)
							unsigned int index = 0;
							while ( Tab::current != Tab::tabsOpen[index] )
								index++;
							sessionStore << index << "\n"; // store position of current tab once found

							while ( Tab::tabsOpen.size() > 0 ) {
								sessionStore << Tab::tabsOpen[0]->file->fullPath << "@" << Tab::tabsOpen[0]->file->getServerIP() << "\n";
								Tab::tabsOpen[0]->closeTab();
							}
						}

						sessionStore.close();
					}
					else
						std::cerr << "failed to save session\n";
					/* ================================ */

					mainWin.close();
				}

				else if ( event.type == sf::Event::MouseWheelMoved ) {
					if ( Tab::current != NULL ) {
						if ( (Tab::current->file->lineRenderStart - event.mouseWheel.delta) < 0 )
							Tab::current->file->lineRenderStart = 0;
						else
							Tab::current->file->lineRenderStart -= event.mouseWheel.delta;
					}
				}

				if ( Tab::tabsOpen.size() > 0 ) {
					save.setUseable( true );
					undo.setUseable( true );
					redo.setUseable( true );
					cut.setUseable( true );
					copy.setUseable( true );
					paste.setUseable( true );
					buildDebug.setUseable( true );
					buildRelease.setUseable( true );
				}
				else {
					save.setUseable( false );
					undo.setUseable( false );
					redo.setUseable( false );
					cut.setUseable( false );
					copy.setUseable( false );
					paste.setUseable( false );
					buildDebug.setUseable( false );
					buildRelease.setUseable( false );
				}

				TextReceiver::checkSwitchReceiver( mainWin );

				bool isText = false;
				for ( unsigned int index = 0 ; !isText && index < TextReceiver::textInterfaces.size() ; index++ ) {
					if ( Window == GetActiveWindow() && DropDown::currentOpen == NULL && TextReceiver::textInterfaces[index]->isHovered( mainWin ) && TextReceiver::textInterfaces[index]->isActive() ) { // FIXME win32 use of HWND and GetActiveWindow()
						textCursor.set( mainWin );
						isText = true;
					}
				}

				if ( !isText ) {
					normalCursor.set( mainWin );
				}

				if ( TextReceiver::currentReceiver != NULL ) {
					TextReceiver::currentReceiver->handleEvent( event );
				}

				Tab::vectorProtect.startReading();
				if ( Tab::tabsOpen.size() > 0 ) {
					/* ===== Switch Tabs If Other Tab Clicked ===== */
					for ( unsigned int index = 0 ; index < Tab::tabsOpen.size() ; index++ ) {
						if ( Tab::tabsOpen[index]->isHovered( mainWin ) && mouseButtonReleased( event , sf::Mouse::Button::Left ) ) {
							Tab::currentProtect.startWriting();
							Tab::current = Tab::tabsOpen[index];
							Tab::currentProtect.stopWriting();
						}
					}
					/* ============================================ */
				}
				Tab::vectorProtect.stopReading();

				for ( unsigned int index = 0 ; index < Button::allButtons.size() ; index++ ) { // handles all hotkeys that are assigned to buttons
					if ( Button::allButtons[index]->hotKeyActivated( event ) ) {
						if ( Button::allButtons[index]->func != NULL )
							Button::allButtons[index]->func();
					}
				}

				/* =============== DropDown Management =============== */
				if ( DropDown::currentOpen == NULL )
					DropDown::checkAllClicked( mainWin );
				else {
					if ( DropDown::currentOpen->open ) {
						// Changes color of button currently hovered over
						for ( unsigned int index = 1 ; index < DropDown::currentOpen->menu.size() ; index++ ) {

							// if button color is wrong for current hover state, switch it to other color
							if ( DropDown::currentOpen->menu[index]->isHovered( mainWin ) ^ ( DropDown::currentOpen->menu[index]->getFillColor().r == BUTTON_HOVER ) )
								DropDown::currentOpen->menu[index]->setFillColor( DropDown::currentOpen->menu[index]->getFillColor().r ^ Button::colorXOR ); // "r" chosen is arbitrary since r, g, and b components are all equal

							// makes current DropDown menu visible if it isn't already
							if ( !DropDown::currentOpen->menu[index]->isVisible() )
								DropDown::currentOpen->menu[index]->setVisible( true );
						}

						// Determines which button from currently open DropDown menu is hovered, if it changed
						if ( currentButtonHovered == NULL ) {
							for ( unsigned int hoverIndex = 0 ; currentButtonHovered == NULL && hoverIndex < DropDown::currentOpen->menu.size() ; hoverIndex++ ) {
								if ( DropDown::currentOpen->menu[hoverIndex]->isHovered( mainWin ) )
									currentButtonHovered = DropDown::currentOpen->menu[hoverIndex];
							}
						}
						else if ( !currentButtonHovered->isHovered( mainWin ) ) {
							currentButtonHovered = NULL;
							for ( unsigned int hoverIndex = 0 ; currentButtonHovered == NULL && hoverIndex < DropDown::currentOpen->menu.size() ; hoverIndex++ ) {
								if ( DropDown::currentOpen->menu[hoverIndex]->isHovered( mainWin ) )
									currentButtonHovered = DropDown::currentOpen->menu[hoverIndex];
							}
						}

						// Changes DropDown menu when cursor is hovered over header of different one
						DropDown::oldOpen = DropDown::currentOpen;
						if ( currentButtonHovered == NULL ) {
							for ( unsigned int index = 0 ; DropDown::oldOpen == DropDown::currentOpen && index < DropDown::dropDowns.size() ; index++ ) {
								if ( DropDown::dropDowns[index]->menu[0]->isHovered( mainWin ) && DropDown::dropDowns[index] != DropDown::currentOpen ) //if hovered over header other than currently open one
									DropDown::currentOpen->switchMenu( DropDown::dropDowns[index] );
							}

							if ( mouseButtonReleased( event , sf::Mouse::Left ) && DropDown::currentOpen == DropDown::oldOpen )
								DropDown::currentOpen->closeMenu();
						}

						// Handles actions that could close the menu
						else if ( DropDown::currentOpen->open ) {
							if ( mouseButtonReleased( event , sf::Mouse::Left ) ) {
								if ( currentButtonHovered == DropDown::currentOpen->menu[0]  ) {
									if ( DropDown::firstUse )
										DropDown::firstUse = false;
									else
										DropDown::currentOpen->closeMenu();
								}

								else if ( !DropDown::firstUse && currentButtonHovered != NULL ) {
									if ( currentButtonHovered->isUseable() && currentButtonHovered->func != NULL ) {
										currentButtonHovered->setFillColor( currentButtonHovered->getFillColor().r ^ Button::colorXOR ); // give button non-hovered color again
										DropDown::currentOpen->closeMenu();
										currentButtonHovered->func();
									}
								}
							}

							if ( DropDown::currentOpen != NULL && keyPressed( event , sf::Keyboard::Escape ) ) //if escape pressed, closes menu
								DropDown::currentOpen->closeMenu();
						}
					}
				}
				/* =================================================== */

				Tab::checkTabClose( mainWin );
				/* ===== Redraw objects ===== */
				mainWin.clear( sf::Color( BACKGROUND_COLOR , BACKGROUND_COLOR , BACKGROUND_COLOR ) );

				editBackground.activate( Tab::tabsOpen.size() > 0 ); // change background of editor

				mainWin.draw( editBackground );

				Tab::currentProtect.startReading();

				if ( Tab::current != NULL ) {
					Tab::current->file->fileProtect.startReading();
					mainWin.draw( *Tab::current->file );
					Tab::current->file->fileProtect.stopReading();

					if ( editBackground.isReceiving() ) // if editor window is receiving keyboard input
						Tab::current->file->drawCursor();
				}

				Tab::currentProtect.stopReading();

				mainWin.draw( mainTools );
				Tab::draw( mainWin );

				mainWin.draw( statusBar );

				if ( chatWindow != NULL )
					mainWin.draw( *chatWindow );

				if ( DropDown::currentOpen != NULL )
					mainWin.draw( *DropDown::currentOpen ); // draw drop-down menu now that its events are processed

				mainWin.display();
				/* ========================== */
			}
		}
	}

	File::unbindSockets();

	// Clean up window
	DestroyWindow( Window );
	UnregisterClass( "AvianCarrier" , Instance );

	fclose( stderr );

	return EXIT_SUCCESS;
}

// FIXME win32 callback function
LRESULT CALLBACK OnEvent( HWND Handle , UINT Message , WPARAM WParam , LPARAM LParam ) {
	// Redraw GUI elements when window is resized or moved
	if ( Message == WM_SIZE || Message == WM_WINDOWPOSCHANGED ) {
		if ( mainWin.isOpen() ) {
			mainWin.setView( sf::View( sf::FloatRect( 0 , 0 , mainWin.getSize().x , mainWin.getSize().y ) ) );
			AutoResize::resizeAll( mainWin );

			Tab::tabBase.setSize( sf::Vector2f( mainWin.getSize().x , Tab::tabBase.getSize().y ) );
			AutoResize::resizeAll( mainWin );
			if ( ChatWindow::getInstance()->isVisible() )
				editBackground.setSize( sf::Vector2f( editBackground.getSize().x - ChatWindow::getInstance()->getSize().x - 1 , editBackground.getSize().y ) );

			/* ===== Redraw objects ===== */
			mainWin.clear( sf::Color( BACKGROUND_COLOR , BACKGROUND_COLOR , BACKGROUND_COLOR ) );

			editBackground.activate( Tab::tabsOpen.size() > 0 ); // change background of editor

			mainWin.draw( editBackground );

			Tab::currentProtect.startReading();

			if ( Tab::current != NULL ) {
				Tab::current->file->fileProtect.startReading();
				mainWin.draw( *Tab::current->file );
				Tab::current->file->fileProtect.stopReading();
			}

			Tab::currentProtect.stopReading();

			mainWin.draw( mainTools );
			Tab::draw( mainWin );

			mainWin.draw( statusBar );

			mainWin.draw( *ChatWindow::getInstance() );

			if ( DropDown::currentOpen != NULL )
				mainWin.draw( *DropDown::currentOpen ); // draw drop-down menu now that its events are processed

			mainWin.display();
			/* ========================== */
		}
	}
	else {
		switch ( Message ) {
		// Quit when we close the main window
		case WM_CLOSE: {
			PostQuitMessage(0);
			break;
		}
		default: {
			return DefWindowProc(Handle, Message, WParam, LParam);
		}
		}
	}

	return 0;
}

enum fileIMG {
	folder = 0,
	cFile,
	cppFile,
	hFile,
	hppFile,
	txtFile,
	unknownFile
};

std::string getExtension( std::string extension ) {
	if ( extension.rfind( "." ) >= extension.length() )
		return ""; // no extension, so it may be a folder
	else // it has an extension
		return extension.substr( extension.rfind( "." ) + 1 ); // extract everything after the last period for the extension
}

std::string changeExtension( std::string file , std::string extension ) {
	return file.substr( 0 , file.rfind( "." ) ) + "." + extension;
}

void closeProgram() {
	CLOSE_THREADS = true;

	mainWin.close();
}

void selectFileIMG( sf::Sprite& spriteOut , std::string name , bool onServer = false );

std::vector<sf::Text>* getTextList( std::vector<std::string>* fileList , std::vector<sf::Sprite>& spriteList , bool onServer = false ) {
	std::vector<sf::Text>* textList = new std::vector<sf::Text>;

	while ( spriteList.size() > 0 )
		spriteList.erase( spriteList.begin() );

	if ( fileList != NULL ) {
		for ( unsigned int index = 0 ; index < fileList->size() ; index++ ) {
			textList->push_back( sf::Text( (*fileList)[index] , Base::segoeUI , 12 ) );

			(*textList)[textList->size() - 1].setPosition( sf::Vector2f( 18 , 18.f * (textList->size() - 1) + 1 ) );
			(*textList)[textList->size() - 1].setColor( sf::Color( 0 , 0 , 0 ) );

			spriteList.push_back( sf::Sprite() );
			selectFileIMG( spriteList.back() , (*fileList)[index] , onServer );
			spriteList.back().setPosition( sf::Vector2f( 1 , ( 16 /*hilight.getSize().y*/ + 2 ) * index + 1 ) );
		}

		return textList;
	}
	else
		return NULL;
}

std::vector<sf::Text>* getLocalTextList( std::string path , std::vector<sf::Sprite>& spriteList ) {
	return getTextList( getList( path ) , spriteList ); // gets list of files in directory and prepares them for drawing
}

std::vector<std::string>* getServerList( std::string path , int& serverErrno ) {
	sf::Packet fileNames;

	fileNames << "dirList" << path; // add data to packet
	if ( File::openSocket.send( fileNames , "127.0.0.1" , File::serverOpen ) == sf::Socket::Done ) {
		sf::IpAddress returnIP;
		unsigned short returnPort;
		if ( File::openSocket.receive( fileNames , returnIP , returnPort ) == sf::Socket::Done ) {
			std::string command;
			fileNames >> command;

			if ( command == "dirList" ) {
				std::string name;
				std::vector<std::string>* tempList = new std::vector<std::string>;

				while ( !fileNames.endOfPacket() ) {
					fileNames >> name;
					tempList->push_back( name );
				}

				serverErrno = 0; // success
				return tempList;
			}
		}
		else {
			serverErrno = 1; // receive failed, most likely from disconnect
		}
	}

	return NULL;
}

std::vector<sf::Text>* getServerTextList( std::string path , int& serverErrno , std::vector<sf::Sprite>& spriteList ) {
	return getTextList( getServerList( path , serverErrno ) , spriteList , true );
}

void selectFileIMG( sf::Sprite& spriteOut , std::string name , bool onServer ) {
	std::string extension = getExtension( name );

	if ( extension == "" ) {
		int errNo = 0;

		if ( onServer ) {
			if ( getServerList( searchDir + "/" + name , errNo ) != NULL )
				spriteOut.setTexture( Base::fileImages[fileIMG::folder] );
			else
				spriteOut.setTexture( Base::fileImages[fileIMG::unknownFile] );
		}
		else {
			if ( getList( searchDir + "/" + name ) != NULL )
				spriteOut.setTexture( Base::fileImages[fileIMG::folder] );
			else
				spriteOut.setTexture( Base::fileImages[fileIMG::unknownFile] );
		}
	}

	else if ( extension == "c" )
		spriteOut.setTexture( Base::fileImages[fileIMG::cFile] );

	else if ( extension == "cpp" )
		spriteOut.setTexture( Base::fileImages[fileIMG::cppFile] );

	else if ( extension == "h" )
		spriteOut.setTexture( Base::fileImages[fileIMG::hFile] );

	else if ( extension == "hpp" )
		spriteOut.setTexture( Base::fileImages[fileIMG::hppFile] );

	else if ( extension == "txt" )
		spriteOut.setTexture( Base::fileImages[fileIMG::txtFile] );
	else
		spriteOut.setTexture( Base::fileImages[fileIMG::unknownFile] );
}

// FIXME win32 openFile uses child window
void openFile() {
	File::bindSockets();

	HWND openHWND = CreateWindow( mainClassName , "Browse For File" , WS_SYSMENU | WS_VISIBLE , mainWin.getPosition().x + ( mainWin.getSize().x - 322 ) / 2 , mainWin.getPosition().y + ( mainWin.getSize().y - 332 ) / 2 , 322 , 332 , mainWin.getSystemHandle() , NULL , globalInstance , NULL );

	sf::RenderWindow openBox( openHWND );
	HWND view = CreateWindow("STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 14,  45, 289, 201, openHWND, NULL, globalInstance, NULL);
	sf::RenderWindow openView( view );

	sf::Image openIcon;
	if ( openIcon.loadFromFile( "Resource/DownloadArrow.png" ) ) {
		openIcon.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );
		openBox.setIcon( openIcon.getSize().x , openIcon.getSize().y , openIcon.getPixelsPtr() );
	}

	ScrollBar scrollBar( 15.f , 199.f );

	Button OK( "OK" , "" , "" , -1 , 50 , []{} );
	Button Cancel( "Cancel" , "" , "" , -1 , 50 , []{} );
	Button prevDir( "Parent Dir" , "" , "" , -1 , 50 , []{} );

	OK.setSize( sf::Vector2f( 72 , OK.getSize().y ) );
	OK.recenterText();

	Cancel.setSize( sf::Vector2f( 72 , Cancel.getSize().y ) );
	Cancel.recenterText();

	scrollBar.setPosition( 273 , 1 );
	OK.setPosition( 149 , 272 );
	Cancel.setPosition( 230 , 272 );
	prevDir.setPosition( 15 , 15 );

	scrollBar.setVisible( true );
	OK.setVisible( true );
	Cancel.setVisible( true );
	prevDir.setVisible( true );

	openView.draw( scrollBar );
	openBox.draw( OK );
	openBox.draw( Cancel );
	openBox.draw( prevDir );

	sf::RectangleShape filePicBackground( sf::Vector2f( 17 , openView.getSize().y ) );
	filePicBackground.setPosition( sf::Vector2f( 0 , 0 ) );
	filePicBackground.setFillColor( sf::Color( 210 , 210 , 210 ) );

	sf::Event event;
	bool newEvent;

	std::vector<sf::Text>* fileList;
	std::vector<sf::Sprite> spriteList;

	int serverErrno = 0;
	fileList = getTextList( getServerList( searchDir , serverErrno ) , spriteList , true );

	int selection = 0;
	int lastSelection = 0;

	sf::RectangleShape hilight( sf::Vector2f( 254.f , 16.f ) );
	hilight.setFillColor( sf::Color( 130 , 130 , 130 ) );
	hilight.setOutlineThickness( 1 );
	hilight.setOutlineColor( sf::Color( 0 , 128 , 225 ) );

	sf::Clock mouseClickTime;
	unsigned char clickCount = 0;

	bool lMouseOldPressed = false;
	bool lMouseNewPressed = false;

	// Create ways to navigate through menu
	MenuSelect keySelect;
	MenuSelect arrowSelect;

	MSG Message;
	while ( openBox.isOpen() && !CLOSE_THREADS ) {
		if ( PeekMessage( &Message , NULL , 0 , 0 , PM_REMOVE ) ) {
			// If a message was waiting in the message queue, process it
			TranslateMessage( &Message );
			DispatchMessage( &Message );
		}
		else {
			newEvent = openBox.pollEvent( event );

			if ( newEvent && event.type == sf::Event::Closed )
				openBox.close();

			openBox.clear( sf::Color( 80 , 80 , 80 ) );
			openView.clear( sf::Color( 255 , 255 , 255 ) );

			openView.draw( filePicBackground );

			// draw contents of view
			if ( fileList != NULL ) { // don't draw anything if there is no list
				for ( unsigned int index = 0 ; index < fileList->size() ; index++ ) {
					openView.draw( spriteList[index] );

					if ( sf::Mouse::getPosition( openView ).y > (*fileList)[index].getPosition().y - 3 && sf::Mouse::getPosition( openView ).y < (*fileList)[index].getPosition().y + hilight.getSize().y ) {
						if ( sf::Mouse::getPosition( openView ).x >= (*fileList)[index].getPosition().x - 2 && sf::Mouse::getPosition( openView ).x < (*fileList)[index].getPosition().x + hilight.getSize().x ) {
							hilight.setOutlineColor( sf::Color( 0 , 128 , 225 ) ); // puts gray outline around text showing cursor is selecting that

							if ( sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) ) { // changes selection to where user clicks
								lMouseNewPressed = true;

								if ( selection >= 0 && selection < static_cast<int>(fileList->size()) )
									(*fileList)[selection].setColor( sf::Color( 0 , 0 , 0 ) );
								selection = index;
							}
							else
								lMouseNewPressed = false;

							/* ===== Detect double clicks ===== */
							if ( !lMouseNewPressed && lMouseOldPressed ) { // if left mouse button released
								if ( clickCount == 0 )
									mouseClickTime.restart();

								clickCount++;
							}

							if ( mouseClickTime.getElapsedTime().asMilliseconds() > 500 && clickCount > 0 )
								clickCount = 0;
							/* ================================ */

							if ( clickCount == 2 ) {
								std::string temp = searchDir + "/" + (*fileList)[selection].getString();

								std::vector<std::string>* tempServerList = getServerList( temp , serverErrno );
								if ( getExtension( (*fileList)[selection].getString() ) == "" && tempServerList != NULL ) {
									delete fileList; // clean up fileList so it can store new data
									searchDir = temp; // update search directory to within folder
									fileList = getTextList( tempServerList , spriteList , true ); // gets list of files in directory and prepares them for drawing

									// clean up tempServerList before it goes out of scope
									delete tempServerList;
									tempServerList = NULL;

									break; // exits for loop so entire vector redraws from beginning

									if ( fileList->size() == 0 )
										selection = -1;
									else
										selection = 0;
								}
								else if ( serverErrno == 0 ) { // if server returned empty list for child directory and connection isn't down (also causes empty directory)
									searchDir = searchDir.substr( 9/* length of "Documents" */ ); //eliminates "Documents" from beginning of file directory because all files are there anyway

									if ( searchDir.substr( 0 , 1 ) == "/" )
										searchDir = searchDir.substr( 1 );

									searchDir = "server:" + searchDir;
									Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , searchDir + "/" + (*fileList)[selection].getString() );
									openBox.close();
								}

								clickCount = 0;
							}

							lMouseOldPressed = lMouseNewPressed;
						}
					}
					else
						hilight.setOutlineColor( sf::Color( 255 , 255 , 255 ) );

					if ( selection == static_cast<int>(index) ) { // selection must be greater than or equal to 0
						(*fileList)[index].setColor( sf::Color( 255 , 255 , 255 ) );
						hilight.setFillColor( sf::Color( 130 , 130 , 130 ) );
					}
					else
						hilight.setFillColor( sf::Color( 255 , 255 , 255 ) );

					hilight.setPosition( sf::Vector2f( 17.f , ( hilight.getSize().y + 2 ) * index + 1 ) );
					openView.draw( hilight );

					openView.draw( (*fileList)[index] );
				}
			}

			if ( scrollBar.topArrow.isHovered( openView ) )
				scrollBar.topArrow.setFillColor( BUTTON_HOVER );
			else
				scrollBar.topArrow.setFillColor( BUTTON_OFF );

			if ( scrollBar.bottomArrow.isHovered( openView ) )
				scrollBar.bottomArrow.setFillColor( BUTTON_HOVER );
			else
				scrollBar.bottomArrow.setFillColor( BUTTON_OFF );

			if ( OK.isHovered( openBox ) )
				OK.setFillColor( BUTTON_HOVER );
			else
				OK.setFillColor( BUTTON_OFF );

			if ( Cancel.isHovered( openBox ) )
				Cancel.setFillColor( BUTTON_HOVER );
			else
				Cancel.setFillColor( BUTTON_OFF );

			if ( prevDir.isHovered( openBox ) )
				prevDir.setFillColor( BUTTON_HOVER );
			else
				prevDir.setFillColor( BUTTON_OFF );

			openView.draw( scrollBar );
			openBox.draw( OK );
			openBox.draw( Cancel );
			openBox.draw( prevDir );

			if ( (OK.isHovered( openBox ) && mouseButtonReleased( event , sf::Mouse::Left )) || (newEvent &&  keyReleased( event , sf::Keyboard::Return )) ) {
				if ( selection >= 0 ) {
					std::string temp = searchDir + "/" + (*fileList)[selection].getString();

					std::vector<std::string>* tempServerList = getServerList( temp , serverErrno );
					if ( getExtension( (*fileList)[selection].getString() ) == "" && tempServerList != NULL ) {
						delete fileList; // clean up fileList so it can store new data
						searchDir = temp; // update search directory with extra folder
						fileList = getTextList( tempServerList , spriteList , true ); // gets list of files in directory and prepares them for drawing

						if ( fileList->size() == 0 )
							selection = -1;
						else
							selection = 0;
					}
					else if ( serverErrno == 0 ) { // if server returned empty list for child directory and connection isn't down (also causes empty directory)
						searchDir = searchDir.substr( 9/* length of "Documents" */ ); //eliminates "Documents" from beginning of file directory because all files are there anyway

						if ( searchDir.substr( 0 , 1 ) == "/" )
							searchDir = searchDir.substr( 1 );

						searchDir = "server:" + searchDir;
						Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , searchDir + "/" + (*fileList)[selection].getString() );
						openBox.close();
					}

					delete tempServerList; // clean up tempServerList before it goes out of scope
				}
			}

			if ( (Cancel.isHovered( openBox ) && mouseButtonReleased( event , sf::Mouse::Left )) || keyPressed( event , sf::Keyboard::Escape ) )
				openBox.close();

			if ( newEvent && ( (prevDir.isHovered( openBox ) && mouseButtonReleased( event , sf::Mouse::Left )) || keyReleased( event , sf::Keyboard::BackSpace ) ) ) {
				if ( searchDir != "Documents" ) {
					delete fileList; // clean up fileList so it can store new data
					searchDir = searchDir = searchDir.substr( 0 , searchDir.rfind( "/" ) ); // update search directory to parent folder
					std::vector<std::string>* tempServerList = getServerList( searchDir , serverErrno );
					fileList = getTextList( tempServerList , spriteList , true ); // gets list of files in directory and prepares them for drawing

					delete tempServerList; // clean up tempServerList before it goes out of scope

					selection = 0;
				}
			}

			while ( openBox.pollEvent( event ) ) {
				if ( event.type == sf::Event::Closed )
					openBox.close();
			}

			if ( fileList == NULL ) { // if couldn't retrieve a file list from the server
				sf::Text serverError( sf::String( "<Cannot connect to server>" ) , Base::segoeUI , 12 );
				serverError.setPosition( sf::Vector2f( 18.f , 1.f ) );
				serverError.setColor( sf::Color( 0 , 0 , 0 ) );
				openView.draw( serverError );
			}
			else {
				keySelect.setSize( fileList->size() );
				arrowSelect.setSize( fileList->size() );
			}

			keySelect.setSelection( selection );
			arrowSelect.setSelection( selection );

			/* ===== Handle selecting item in menu : KEYS ===== */
			if ( keySelect.updateMove( sf::Keyboard::isKeyPressed( sf::Keyboard::Up ) , sf::Keyboard::isKeyPressed( sf::Keyboard::Down ) ) ) {

				lastSelection = keySelect.getLastSelection();
				if ( lastSelection >= 0 && lastSelection < static_cast<int>(fileList->size()) )
					(*fileList)[selection].setColor( sf::Color( 0 , 0 , 0 ) );
				selection = keySelect.getSelection();
			}
			/* ================================================ */

			/* ===== Handle selecting item in menu : ARROWS ===== */
			else if ( arrowSelect.updateMove( scrollBar.topArrow.isClicked( openView ) , scrollBar.bottomArrow.isClicked( openView ) ) ) {

				lastSelection = arrowSelect.getLastSelection();
				if ( lastSelection >= 0 && lastSelection < static_cast<int>(fileList->size()) )
					(*fileList)[selection].setColor( sf::Color( 0 , 0 , 0 ) );
				selection = arrowSelect.getSelection();
			}
			/* ================================================== */

			openBox.display();
			openView.display();
		}
	}

	searchDir = "Documents"; // reset starting directory in open dialog

	DestroyWindow( openView.getSystemHandle() );
	DestroyWindow( openHWND );
}

void copyText() {
	if ( OpenClipboard( NULL ) ) {
		SetClipboardData( CF_TEXT , static_cast<void*>( const_cast<char*>("") ) ); // stores hilighted text here
		CloseClipboard();
	}
}

void pasteText() {
	if ( OpenClipboard( NULL ) ) {
		std::string text = static_cast<char*>( GetClipboardData( CF_TEXT ) );
		CloseClipboard();

		Tab::current->file->insert( text );
	}
}

void buildEXE() {
	Tab::currentProtect.startReading();
	Tab::current->saveLocal();
	Tab::currentProtect.stopReading();
	statusBar.setStatus( "Building..." );
	mainWin.draw( statusBar );
	mainWin.display();

	std::string tempDir = searchDir;
	if ( !sysRun( "C:/Users/Tyler/Downloads/EclipseCDT/mingw/bin/g++.exe" , "-O3 -Wall -c -fmessage-length=0 -std=c++0x -o " + tempDir + "/" + changeExtension( Tab::current->file->fileName , "o" ) + " " + tempDir + "/" + changeExtension( Tab::current->file->fileName , "cpp" ) ) )
		sysRun( "C:/Users/Tyler/Downloads/EclipseCDT/mingw/bin/g++.exe" , "-s -static -o " + tempDir + "/" + changeExtension( Tab::current->file->fileName , "exe" ) + " " + tempDir + "/" + changeExtension( Tab::current->file->fileName , "o" ) );

	statusBar.setStatus( "Done" );
}

void displayAbout() {
	sf::SplashScreen splash( 400 , 250 , "" , sf::Style::Titlebar , "PigeotSplash.png" );
	splash.display();

	while ( splash.isOpen() && !CLOSE_THREADS ) {
		splash.checkForExitClick();
	}
}

// FIXME win32 sysRun creates process with Win32 API
int sysRun( std::string programAndDir , std::string args ) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process.
	if( !CreateProcess( programAndDir.c_str(),  // No module name (use command line)
		const_cast<char*>( args.c_str() ),      // Command line
		NULL,                                   // Process handle not inheritable
		NULL,                                   // Thread handle not inheritable
		TRUE,                                   // Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,                     // Open program in new console window
		NULL,                                   // Use parent's environment block
		NULL,                                   // Use parent's starting directory
		&si,                                    // Pointer to STARTUPINFO structure
		&pi )                                   // Pointer to PROCESS_INFORMATION structure
	)
	{
		std::cerr << "CreateProcess failed: ";

		int error = GetLastError();
		if ( error == 2 ) {
			std::cerr << "Application '" << programAndDir << "' not found\n";
		}
        else if ( error == 3 ) {
			std::cerr << "Path not found\n";
        }
        else {
			std::cerr << "Error " << error << '\n';
        }

		return EXIT_FAILURE;
	}

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );

	// Close process and thread handles.
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return EXIT_SUCCESS;
}
