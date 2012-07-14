//=============================================================================
//File Name: MainEditor.cpp
//Description: Handles main editor and window
//Author: Tyler Veness
//Last Modified: 7/14/2012
//Version: 0.1
//=============================================================================

//TODO: Implement text selection with highlighting (Ctrl + A flag will be special case of this) -> send this to copy function

//TODO: Progress Bars (not needed yet)
//TODO: Save Dialog
//TODO: Ability to tab through buttons (button outline will be blue)

#include <iostream>//FIXME

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Thread.hpp>

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
#include "DirList.h"
#include "MenuSelect.h"

void closeProgram();
void openFile();
void pullFile();
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
bool CLOSE_THREADS = false;

Base temp; //loads all files for base class

StatusBar statusBar;

std::string searchDir;

bool updateSizes = false;

LRESULT CALLBACK OnEvent( HWND Handle , UINT Message , WPARAM WParam , LPARAM LParam );

void syncServer() {
	while ( !CLOSE_THREADS ) {
		Tab::tabMutex.lock();

		if ( Tab::current != NULL ) {
			Tab::current->file->sendToIP();
			Tab::current->file->receiveFromAny();
		}

		Tab::tabMutex.unlock();

		sf::sleep( sf::Time( 500000 ) );
	}
}

Button file( "File" , "" , "" , -1 , 30 , []{} );
Button newFile( "New" , "Ctrl + N" , "Control N" , -1 , 40 , []{ Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , 50001 ); } );
Button open( "Open" , "Ctrl + O" , "Control O" , -1 , 40 , []{ openThread.launch(); } );
Button save( "Save" , "Ctrl + S" , "Control S" , -1 , 40 , []{ Tab::current->saveLocal(); } , false );
Button pull( "Pull" , "Ctrl + P" , "Control P" , -1 , 40 , []{ pullFile(); } , false );
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

//Button collaborate( "Collaborate" , "" , "" , -1 , 30 , []{} );
//Button chat( "Chat" , "Ctrl + T" , "Control T" , -1 , 40 , []{ ChatWindow::toggleChat(); } );

Button options( "Options" , "" , "" , -1 , 30 , []{} );
Button settings( "Settings..." , " " , "" , -1 , 40 , []{} , false );
Button about( "About" , " " , "" , -1 , 40 , []{ aboutThread.launch(); } );

DropDown fileMenu( 0 , 4 , { &file , &newFile , &open , &save , &pull , &exitButton } );
DropDown editMenu( 0 , 4 , { &edit , &undo , &redo , &cut , &copy , &paste } );
DropDown buildMenu( 0 , 4 , { &build , &buildDebug , &buildRelease } );
//DropDown collaborateMenu( 0 , 4 , { &collaborate , &chat } );
DropDown optionMenu( 0 , 4 , { &options , &settings , &about } );

Toolbar mainTools( 0.f , 0.f , mainWin.getSize().x , 24.f , { &fileMenu , &editMenu , &buildMenu /*, &collaborateMenu*/ , &optionMenu } );

Editor editBackground;

INT WINAPI WinMain( HINSTANCE Instance , HINSTANCE , LPSTR , INT ) {
#if 0
	Button file( "File" , "" , "" , -1 , 30 , []{} );
	Button newFile( "New" , "Ctrl + N" , "Control N" , -1 , 40 , []{ Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , 50001 ); } );
	Button open( "Open" , "Ctrl + O" , "Control O" , -1 , 40 , []{ openThread.launch(); } );
	Button save( "Save" , "Ctrl + S" , "Control S" , -1 , 40 , []{ Tab::current->saveLocal(); } , false );
	Button pull( "Pull" , "Ctrl + P" , "Control P" , -1 , 40 , []{ pullFile(); } , false );
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

	//Button collaborate( "Collaborate" , "" , "" , -1 , 30 , []{} );
	//Button chat( "Chat" , "Ctrl + T" , "Control T" , -1 , 40 , []{ ChatWindow::toggleChat(); } );

	Button options( "Options" , "" , "" , -1 , 30 , []{} );
	Button settings( "Settings..." , " " , "" , -1 , 40 , []{} , false );
	Button about( "About" , " " , "" , -1 , 40 , []{ aboutThread.launch(); } );

	DropDown fileMenu( 0 , 4 , { &file , &newFile , &open , &save , &pull , &exitButton } );
	DropDown editMenu( 0 , 4 , { &edit , &undo , &redo , &cut , &copy , &paste } );
	DropDown buildMenu( 0 , 4 , { &build , &buildDebug , &buildRelease } );
	//DropDown collaborateMenu( 0 , 4 , { &collaborate , &chat } );
	DropDown optionMenu( 0 , 4 , { &options , &settings , &about } );

	Toolbar mainTools( 0.f , 0.f , mainWin.getSize().x , 24.f , { &fileMenu , &editMenu , &buildMenu /*, &collaborateMenu*/ , &optionMenu } );

	Editor editBackground;
#endif

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

	HWND Window = CreateWindow( mainClassName , "Avian Carrier" , WS_SYSMENU | WS_VISIBLE | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME , centerWin.getPosition().x - ( 800 - centerWin.getSize().x ) / 2 , centerWin.getPosition().y - ( 600 - centerWin.getSize().y ) / 2 , 800 , 600 , NULL , NULL , Instance , NULL );

	centerWin.close();
	//mainWin.create( sf::VideoMode() , "Avian Carrier" , sf::Style::Resize | sf::Style::Close );
	mainWin.create( Window );

	sf::Image appIcon;
	if ( !appIcon.loadFromFile( "Resource/Pigeot.png" ) )
		return EXIT_FAILURE;
	appIcon.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );

	mainWin.setIcon( appIcon.getSize().x , appIcon.getSize().y , appIcon.getPixelsPtr() );

	searchDir = "Documents";

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
						Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , 50001 , temp );
				}

				Tab::tabMutex.lock();
				Tab::current = Tab::tabsOpen[currentTabPos]; // restore which tab is current from last session
				Tab::tabMutex.unlock();
			}
		}
	}
	else
		std::cerr << "'session.txt' failed to open\n";
	/* ==================================== */

	sf::Event event;

	Button* currentButtonHovered = NULL;

	mainTools.setVisible( true );

	Tab::draw( mainWin );

	TextReceiver::setReceiver( &editBackground );

	sf::Thread networkThread( syncServer );
	networkThread.launch();

	MSG Message; // FIXME win32 MSG
	while ( mainWin.isOpen() ) {
		// FIXME win32 message handler
		if ( PeekMessage( &Message , NULL , 0 , 0 , PM_REMOVE ) ) {
			// If a message was waiting in the message queue, process it
			TranslateMessage( &Message );
			DispatchMessage( &Message );

			if ( updateSizes ) {
				Tab::tabBase.setSize( sf::Vector2f( mainWin.getSize().x , Tab::tabBase.getSize().y ) );
				AutoResize::resizeAll( mainWin );
				if ( chatWindow->isVisible() )
					editBackground.setSize( sf::Vector2f( editBackground.getSize().x - chatWindow->getSize().x - 1 , editBackground.getSize().y ) );

				/* ===== Redraw objects ===== */
				mainWin.clear( sf::Color( BACKGROUND_COLOR , BACKGROUND_COLOR , BACKGROUND_COLOR ) );

				editBackground.activate( Tab::tabsOpen.size() > 0 ); // change background of editor

				mainWin.draw( editBackground );

				Tab::tabMutex.lock();

				if ( Tab::current != NULL )
					mainWin.draw( *Tab::current->file );

				Tab::tabMutex.unlock();

				mainWin.draw( mainTools );
				Tab::draw( mainWin );

				mainWin.draw( statusBar );

				mainWin.draw( *ChatWindow::getInstance() );

				if ( DropDown::currentOpen != NULL )
					mainWin.draw( *DropDown::currentOpen ); // draw drop-down menu now that its events are processed

				mainWin.display();
				/* ========================== */

				updateSizes = false;
			}
		}
		else {
			while ( mainWin.pollEvent( event ) ) {
				if ( event.type == sf::Event::Closed ) {
					/* ===== Save current session ===== */
					std::ofstream sessionStore( "Config/session.txt" , std::ios_base::trunc );
					if ( sessionStore.is_open() ) {
						Tab::tabMutex.lock();

						if ( Tab::tabsOpen.size() > 0 ) { // if there is a current tab (which means there are also tabs to save)
							unsigned int index = 0;
							while ( Tab::current != Tab::tabsOpen[index] )
								index++;
							sessionStore << index << "\n"; // store position of current tab once found

							while ( Tab::tabsOpen.size() > 0 ) {
								sessionStore << Tab::tabsOpen[0]->file->fullPath << "\n";
								Tab::tabsOpen[0]->closeTab();
							}
						}

						Tab::tabMutex.unlock();
						sessionStore.close();
					}
					else
						std::cerr << "failed to save session\n";
					/* ================================ */

					closeProgram();
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
					pull.setUseable( true );
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
					pull.setUseable( false );
					undo.setUseable( false );
					redo.setUseable( false );
					cut.setUseable( false );
					copy.setUseable( false );
					paste.setUseable( false );
					buildDebug.setUseable( false );
					buildRelease.setUseable( false );
				}

				TextReceiver::checkSwitchReceiver( mainWin );

				if ( TextReceiver::currentReceiver != NULL )
					TextReceiver::currentReceiver->handleEvent( event );

				if ( Tab::tabsOpen.size() > 0 ) {
					/* ===== Switch Tabs If Other Tab Clicked ===== */
					for ( unsigned int index = 0 ; index < Tab::tabsOpen.size() ; index++ ) {
						if ( Tab::tabsOpen[index]->isHovered( mainWin ) && mouseButtonReleased( event , sf::Mouse::Button::Left ) ) {
							Tab::tabMutex.lock();
							Tab::current = Tab::tabsOpen[index];
							Tab::tabMutex.unlock();
						}
					}
					/* ============================================ */
				}

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

							if ( DropDown::currentOpen->menu[index]->isHovered( mainWin ) ^ ( DropDown::currentOpen->menu[index]->getFillColor().r == BUTTON_HOVER ) )
								DropDown::currentOpen->menu[index]->setFillColor( DropDown::currentOpen->menu[index]->getFillColor().r ^ Button::colorXOR );
							else
								DropDown::currentOpen->menu[index]->setFillColor( DropDown::currentOpen->menu[index]->getFillColor() );

							DropDown::currentOpen->menu[index]->setVisible( true );
						}

						// Determines if any button from currently open DropDown menu is pressed
						currentButtonHovered = NULL;
						for ( unsigned int hoverIndex = 0 ; currentButtonHovered == NULL && hoverIndex < DropDown::currentOpen->menu.size() ; hoverIndex++ ) {
							if ( DropDown::currentOpen->menu[hoverIndex]->isHovered( mainWin ) )
								currentButtonHovered = DropDown::currentOpen->menu[hoverIndex];
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

				Tab::tabMutex.lock();

				if ( Tab::current != NULL ) {
					mainWin.draw( *Tab::current->file );

					if ( editBackground.isReceiving() ) {
						sf::Text drawMe( "" , Base::consolas , 12 );
						drawMe.setColor( sf::Color( 0 , 0 , 0 ) );

						/* ===== Draw cursor ===== */
						drawMe.setString( "_" );

						unsigned int xPos = 0;
						for ( unsigned int index = 0 ; index < Tab::current->file->cursorPos.x ; index++ ) {
							if ( Tab::current->file->input.at(Tab::current->file->cursorPos.y).substr( index , 1 ) == "\t" )
								xPos += 4;
							else
								xPos++;
						}
						drawMe.setPosition( 5 + 7 * xPos , Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 2 + 12 * (Tab::current->file->cursorPos.y - Tab::current->file->lineRenderStart) );
						mainWin.draw( drawMe );
						/* ======================= */
					}
				}

				Tab::tabMutex.unlock();

				mainWin.draw( mainTools );
				Tab::draw( mainWin );

				mainWin.draw( statusBar );

				mainWin.draw( *chatWindow );

				if ( DropDown::currentOpen != NULL )
					mainWin.draw( *DropDown::currentOpen ); // draw drop-down menu now that its events are processed

				mainWin.display();
				/* ========================== */
			}

			Tab::checkTabClose( mainWin );
			/* ===== Redraw objects ===== */
			mainWin.clear( sf::Color( BACKGROUND_COLOR , BACKGROUND_COLOR , BACKGROUND_COLOR ) );

			editBackground.activate( Tab::tabsOpen.size() > 0 ); // change background of editor

			mainWin.draw( editBackground );

			Tab::tabMutex.lock();

			if ( Tab::current != NULL ) {
				mainWin.draw( *Tab::current->file );

				if ( editBackground.isReceiving() ) {
					sf::Text drawMe( "" , Base::consolas , 12 );
					drawMe.setColor( sf::Color( 0 , 0 , 0 ) );

					/* ===== Draw cursor ===== */
					drawMe.setString( "_" );

					unsigned int xPos = 0;
					for ( unsigned int index = 0 ; index < Tab::current->file->cursorPos.x ; index++ ) {
						if ( Tab::current->file->input.at(Tab::current->file->cursorPos.y).substr( index , 1 ) == "\t" )
							xPos += 4;
						else
							xPos++;
					}
					drawMe.setPosition( 5 + 7 * xPos , Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 2 + 12 * (Tab::current->file->cursorPos.y - Tab::current->file->lineRenderStart) );
					mainWin.draw( drawMe );
					/* ======================= */
				}
			}

			Tab::tabMutex.unlock();

			mainWin.draw( mainTools );
			Tab::draw( mainWin );

			mainWin.draw( statusBar );

			mainWin.draw( *chatWindow );

			if ( DropDown::currentOpen != NULL )
				mainWin.draw( *DropDown::currentOpen ); // draw drop-down menu now that its events are processed

			mainWin.display();
			/* ========================== */
		}

		sf::sleep( sf::Time( 1000 ) );
	}

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
		mainWin.setView( sf::View( sf::FloatRect( 0 , 0 , mainWin.getSize().x , mainWin.getSize().y ) ) );
		AutoResize::resizeAll( mainWin );
		updateSizes = true;
	}
	else {
		switch ( Message ) {
		// Quit when we close the main window
		case WM_CLOSE: {
			PostQuitMessage(0);
			return 0;
		}
		}
	}

	return DefWindowProc(Handle, Message, WParam, LParam);
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

void selectFileIMG( sf::Sprite& spriteOut , std::string name ) {
	std::string extension = getExtension( name );

	if ( extension == "" ) {
		if ( getList( searchDir + "/" + name ) != NULL )
			spriteOut.setTexture( Base::fileImages[fileIMG::folder] );
		else
			spriteOut.setTexture( Base::fileImages[fileIMG::unknownFile] );
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

std::vector<sf::Text>* getTextList( std::string path ) {
	std::vector<std::string>* fileListTemp = getList( path ); // gets list of files in directory and prepares them for drawing
	std::vector<sf::Text>* fileList = new std::vector<sf::Text>;
	for ( unsigned int index = 0 ; index < fileListTemp->size() ; index++ ) {
		fileList->push_back( sf::Text( (*fileListTemp)[index] , Base::segoeUI , 12 ) );

		(*fileList)[fileList->size() - 1].setPosition( sf::Vector2f( 18 , 18.f * (fileList->size() - 1) + 1 ) );
		(*fileList)[fileList->size() - 1].setColor( sf::Color( 0 , 0 , 0 ) );
	}

	return fileList;
}

void closeProgram() {
	Tab::tabMutex.unlock();
	CLOSE_THREADS = true;

	mainWin.close();
}

// FIXME win32 openFile uses child window
void openFile() {
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

	sf::Sprite imageSprite;

	std::vector<sf::Text>* fileList = getTextList( searchDir ); // gets list of files in directory and prepares them for drawing

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
	while ( openBox.isOpen() ) {
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
					imageSprite.setPosition( sf::Vector2f( 1 , ( hilight.getSize().y + 2 ) * index + 1 ) );

					selectFileIMG( imageSprite , (*fileList)[index].getString() );
					openView.draw( imageSprite );

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

								if ( getExtension( (*fileList)[selection].getString() ) == "" && getList( temp ) != NULL ) {
									delete fileList;
									searchDir = temp; // update search directory to within folder
									fileList = getTextList( temp ); // gets list of files in directory and prepares them for drawing
									break; // exits for loop so entire vector redraws from beginning

									if ( fileList->size() == 0 )
										selection = -1;
									else
										selection = 0;
								}
								else {
									searchDir = searchDir.substr( 9/* length of "Documents" */ ); //eliminates "Documents" from beginning of file directory because all files are there anyway

									if ( searchDir.substr( 0 , 1 ) == "/" )
										searchDir = searchDir.substr( 1 );
									Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , 50001 , searchDir + "/" + (*fileList)[selection].getString() );
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

					if ( getExtension( (*fileList)[selection].getString() ) == "" && getList( temp ) != NULL ) {
						delete fileList;
						searchDir = temp; // update search directory with extra folder
						fileList = getTextList( temp ); // gets list of files in directory and prepares them for drawing

						if ( fileList->size() == 0 )
							selection = -1;
						else
							selection = 0;
					}
					else {
						searchDir = searchDir.substr( 9/* length of "Documents" */ ); //eliminates "Documents" from beginning of file directory because all files are there anyway

						if ( searchDir.substr( 0 , 1 ) == "/" )
							searchDir = searchDir.substr( 1 );
						Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , 50001 , searchDir + "/" + (*fileList)[selection].getString() );
						openBox.close();
					}
				}
			}

			if ( (Cancel.isHovered( openBox ) && mouseButtonReleased( event , sf::Mouse::Left )) || keyPressed( event , sf::Keyboard::Escape ) )
				openBox.close();

			if ( newEvent && ( (prevDir.isHovered( openBox ) && mouseButtonReleased( event , sf::Mouse::Left )) || keyReleased( event , sf::Keyboard::BackSpace ) ) ) {
				if ( searchDir != "Documents" ) {
					delete fileList;
					searchDir = searchDir = searchDir.substr( 0 , searchDir.rfind( "/" ) ); // update search directory to parent folder
					fileList = getTextList( searchDir ); // gets list of files in directory and prepares them for drawing

					selection = 0;
				}
			}

			while ( openBox.pollEvent( event ) ) {
				if ( event.type == sf::Event::Closed )
					openBox.close();
			}

			keySelect.setSize( fileList->size() );
			arrowSelect.setSize( fileList->size() );

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

void pullFile() {

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
	Tab::current->saveLocal();
	statusBar.setStatus( "Building..." );
	mainWin.draw( statusBar );
	mainWin.display();

	std::string tempDir = searchDir;
	std::cout << "\n>";
	if ( !sysRun( "C:/Users/Tyler/Downloads/EclipseCDT/mingw/bin/g++.exe" , "-O3 -Wall -c -fmessage-length=0 -std=c++0x -o " + tempDir + "/" + changeExtension( Tab::current->file->fileName , "o" ) + " " + tempDir + "/" + changeExtension( Tab::current->file->fileName , "cpp" ) ) ) {
		std::cout << "\n>";
		sysRun( "C:/Users/Tyler/Downloads/EclipseCDT/mingw/bin/g++.exe" , "-s -static -o " + tempDir + "/" + changeExtension( Tab::current->file->fileName , "exe" ) + " " + tempDir + "/" + changeExtension( Tab::current->file->fileName , "o" ) );
		std::cout << "\n>";
	}

	statusBar.setStatus( "Done" );
}

void displayAbout() {
	sf::SplashScreen splash( 400 , 250 , "" , sf::Style::Titlebar , "Pigeot Splash.png" );

	sf::Texture sfmlLogo;
	if ( sfmlLogo.loadFromFile( "Resource/sfml.png" ) ) {
		sf::Sprite sfml( sfmlLogo );
		sfml.setPosition( sf::Vector2f( splash.getSize().x - sfml.getTexture()->getSize().x - 8.f , splash.getSize().y - sfml.getTexture()->getSize().y - 8.f ) );
		splash.draw( sfml );
		splash.display();
	}

	splash.waitForExitClick();
}

// FIXME win32 sysRun creates process with Win32 API
int sysRun( std::string programAndDir , std::string args ) {
	std::cout << programAndDir << " " << args << "\n";

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
		std::cout << "CreateProcess failed: ";

		int error = GetLastError();
		if ( error == 2 )
			std::cout << "Application '" << programAndDir << "' not found\n";
		else if ( error == 3 )
			std::cout << "Path not found\n";
		else
			std::cout << "Error " << error << '\n';

		return EXIT_FAILURE;
	}

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, INFINITE );

	// Close process and thread handles.
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return EXIT_SUCCESS;
}
