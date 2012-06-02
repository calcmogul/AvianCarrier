//=============================================================================
//File Name: MainEditor.cpp
//Description: Handles main editor and window
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

//TODO: Implement text selection with highlighting (Ctrl + A flag will be special case of this) -> send this to copy function

//TODO: Progress Bars
//TODO: Ability to tab through buttons (button outline will be blue)

#include <iostream>
#include "Base.h"

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Thread.hpp>

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

bool SetCurrentDirectoryA( std::string dir );
void closeProgram();
void openFile();
void pullFile();
void copyText();
void pasteText();
void buildEXE();
void startChat();
void displayAbout();
int sysRun( std::string programAndDir , std::string args );

sf::Thread openThread( openFile );
sf::Thread buildThread( buildEXE );
sf::Thread aboutThread( displayAbout );

sf::RenderWindow mainWin;
HINSTANCE globalInstance;
MSG Message;
sf::Mutex globalMutex;
bool CLOSE_THREADS = false;

Base temp; //loads all files for base class

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

Button collaborate( "Collaborate" , "" , "" , -1 , 30 , []{} );
Button chat( "Chat" , "Ctrl + T" , "Control T" , -1 , 40 , []{ startChat(); } );

Button options( "Options" , "" , "" , -1 , 30 , []{} );
Button settings( "Settings..." , " " , "" , -1 , 40 , []{} , false );
Button about( "About" , " " , "" , -1 , 40 , []{ aboutThread.launch(); } );

DropDown fileMenu( 0 , 4 , { &file , &newFile , &open , &save , &pull , &exitButton } );
DropDown editMenu( 0 , 4 , { &edit , &undo , &redo , &cut , &copy , &paste } );
DropDown buildMenu( 0 , 4 , { &build , &buildDebug , &buildRelease } );
DropDown collaborateMenu( 0 , 4 , { &collaborate , &chat } );
DropDown optionMenu( 0 , 4 , { &options , &settings , &about } );

Toolbar mainTools( 0.f , 0.f , mainWin.getSize().x , 24.f , { &fileMenu , &editMenu , &buildMenu , &collaborateMenu , &optionMenu } );

Editor editBackground;

StatusBar statusBar;

sf::Clock tabCloseWait;

ChatWindow chatDialog( sf::Vector2f( 200.f , 600.f ) , { sf::Color( 45 , 45 , 45 ) , sf::Color( 100 , 100 , 100 ) , sf::Color( 100 , 100 , 100 ) , sf::Color( 45 , 45 , 45 ) } );

std::string rootDirectory;
std::string searchDir;

void drawObjects() {
	mainWin.clear( sf::Color( BACKGROUND_COLOR , BACKGROUND_COLOR , BACKGROUND_COLOR ) );

	editBackground.activate( Tab::tabsOpen.size() > 0 ); // change background of editor

	mainWin.draw( editBackground );

	globalMutex.lock();

	if ( Tab::current != NULL )
		mainWin.draw( *Tab::current->file );

	globalMutex.unlock();

	mainWin.draw( mainTools );
	Tab::draw( mainWin );

	mainWin.draw( statusBar );

	mainWin.draw( chatDialog );

	if ( DropDown::currentOpen != NULL )
		mainWin.draw( *DropDown::currentOpen ); // draw drop-down menu now that its events are processed

	// Close any tabs that had their X clicked
	for ( unsigned int index = 0 ; index < Tab::tabsOpen.size() ; index++ ) {
		if ( tabCloseWait.getElapsedTime().asMilliseconds() > 200 && mousePressed( sf::Mouse::Left ) && Tab::tabsOpen[index]->isXHovered( mainWin ) ) {
			Tab::tabsOpen[index]->closeTab();

			tabCloseWait.restart();
			break;
		}
	}

	mainWin.display();
}

LRESULT CALLBACK OnEvent(HWND Handle, UINT Message, WPARAM WParam, LPARAM LParam) {
	// Redraw GUI elements when window is resized or moved
	if ( Message == WM_SIZE || Message == WM_WINDOWPOSCHANGED ) {
		mainWin.setView( sf::View( sf::FloatRect( 0 , 0 , mainWin.getSize().x , mainWin.getSize().y ) ) );

		editBackground.setSize( sf::Vector2f( mainWin.getSize().x , statusBar.getPosition().y - 2 - (Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 1) ) );

		mainTools.updateSize( mainWin );
		Tab::tabBase.setSize( sf::Vector2f( mainWin.getSize().x , Tab::tabBase.getSize().y ) );
		statusBar.updateSize( mainWin );
		chatDialog.updateSize( mainWin );

		drawObjects();
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

void syncServer() {
	while ( !CLOSE_THREADS ) {
		if ( Tab::current != NULL ) {
			globalMutex.lock();
			Tab::current->file->sendToIP();
			Tab::current->file->receiveFromAny();
			globalMutex.unlock();
		}

		sf::sleep( sf::Time( 100000 ) );
	}
}

INT WINAPI WinMain( HINSTANCE Instance , HINSTANCE , LPSTR , INT ) {
	freopen( "error.log" , "w+" , stderr );

	// Define a class for our main window
	WNDCLASS WindowClass;
	WindowClass.style         = 0;
	WindowClass.lpfnWndProc   = &OnEvent;
	WindowClass.cbClsExtra    = 0;
	WindowClass.cbWndExtra    = 0;
	WindowClass.hInstance     = Instance;
	WindowClass.hIcon         = NULL;
	WindowClass.hCursor       = 0;
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

	char currentDirectory[GetCurrentDirectoryA( 0 , NULL )];
	GetCurrentDirectoryA( sizeof(currentDirectory) , currentDirectory );
	rootDirectory = currentDirectory;
	searchDir = rootDirectory + "\\Documents";

	sf::Event event;

	Button* currentButtonHovered = NULL;

	statusBar.updateSize( mainWin );
	editBackground.setSize( sf::Vector2f( mainWin.getSize().x , statusBar.getPosition().y - 2 - (Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 1) ) );
	editBackground.setPosition( sf::Vector2f( 0 , Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 2 ) );

	mainTools.setVisible( true );

	Tab::tabBase.setSize( sf::Vector2f( mainWin.getSize().x , Tab::tabBase.getSize().y ) );
	mainTools.updateSize( mainWin );

	sf::Thread networkThread( syncServer );
	networkThread.launch();

	while ( mainWin.isOpen() ) {
		if ( PeekMessage( &Message , NULL , 0 , 0 , PM_REMOVE ) ) {
			// If a message was waiting in the message queue, process it
			TranslateMessage( &Message );
			DispatchMessage( &Message );
		}
		else {
			while ( mainWin.pollEvent( event ) ) {
				if ( event.type == sf::Event::Closed ) {
					// TODO save/close open files here
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

				if ( TextReceiver::currentReceiver != NULL ) {
					std::cout << "Receiver!= NULL\n";
					TextReceiver::currentReceiver->handleEvent( event );
				}
				else
					std::cout << "Receiver=NULL\n";

				if ( Tab::tabsOpen.size() > 0 ) {
					/* ===== Switch Tabs If Other Tab Clicked ===== */
					for ( unsigned int index = 0 ; index < Tab::tabsOpen.size() ; index++ ) {
						if ( Tab::tabsOpen[index]->isHovered( mainWin ) && mouseButtonReleased( event , sf::Mouse::Button::Left ) ) {
							globalMutex.lock();
							Tab::current = Tab::tabsOpen[index];
							globalMutex.unlock();
						}
					}
					/* ============================================ */
#if 0
					/* ===== Handle selecting item in menu : ARROW ===== *///FIXME Delay here
					if ( !(vScrollEdit.topArrow.isPressed( mainWin ) || scrollBar.bottomArrow.isPressed( mainWin )) )
						scrollArrowWaitTime = 0;

					if ( scrollArrowRepeatTime.getElapsedTime().asMilliseconds() >= scrollArrowWaitTime ) {
						upNewARROWPressed = scrollBar.topArrow.isPressed( openView );
						downNewARROWPressed = scrollBar.bottomArrow.isPressed( openView );

						if ( (!upOldARROWPressed && upNewARROWPressed) || (!downOldARROWPressed && downNewARROWPressed) )
							scrollArrowWaitTime = 400;
						else if ( (upNewARROWPressed && upOldARROWPressed) || (downNewARROWPressed && downOldARROWPressed) )
							scrollArrowWaitTime = 100;
						else if ( !(upNewARROWPressed || downNewARROWPressed) )
							scrollArrowWaitTime = 0;

						if ( upNewARROWPressed || downNewARROWPressed ) {
							scrollArrowRepeatTime.restart();

							if ( selection >= 0 && selection < static_cast<int>(fileList->size()) )
								(*fileList)[selection].setColor( sf::Color( 0 , 0 , 0 ) );
						}

						if ( upNewARROWPressed ) {
							if ( selection > 0 )
								selection--;
							else
								selection = fileList->size() - 1;
						}

						else if ( downNewARROWPressed ) {
							if ( selection < static_cast<int>(fileList->size()) - 1 )
								selection++;
							else
								selection = 0;
						}

						upOldARROWPressed = upNewARROWPressed;
						downOldARROWPressed = downNewARROWPressed;
					}
					/* ================================================= */
#endif
					globalMutex.lock();

#if 0
					if ( event.type == sf::Event::TextEntered ) {
						if ( chatDialog.isVisible() )
							chatDialog.passEvent( event );

						if ( event.text.unicode == 8 ) { //pressed Backspace
							if ( Tab::current->file->cursorPos.y > 0 || Tab::current->file->cursorPos.x > 0 ) { //and cursor isn't at beginning of doc
								if ( Tab::current->file->cursorPos.x == 0 ) { //if a newline will be erased
									Tab::current->file->cursorPos.x = Tab::current->file->at( Tab::current->file->cursorPos.y - 1 ).length(); //move cursor to where it would be after line merging is done
									Tab::current->file->at( Tab::current->file->cursorPos.y - 1 ) += Tab::current->file->at( Tab::current->file->cursorPos.y ); //since newline was eliminated, move contents of that line to line before it

									Tab::current->file->erase( Tab::current->file->cursorPos.y ); //erase the line now that the content is moved

									Tab::current->file->cursorPos.y--;
								}
								else { //otherwise do normal character deletion
									if ( Tab::current->file->getCurrentCharacters( 1 , -1 ) == "{" )
										Tab::current->file->bracketMatch--;
									if ( Tab::current->file->getCurrentCharacters( 1 , -1 ) == "}" ) {
										Tab::current->file->bracketMatch++;
										Tab::current->file->tabSpaceCount++;
									}

									Tab::current->file->getCurrentLine() = Tab::current->file->getCurrentLine().substr( 0 , Tab::current->file->cursorPos.x - 1 ) + Tab::current->file->getCurrentLine().substr( Tab::current->file->cursorPos.x , Tab::current->file->getCurrentLine().length() - Tab::current->file->cursorPos.x );
									Tab::current->file->cursorPos.x--;
								}
							}
						}

						else if ( keyPressed( sf::Keyboard::Return ) ) { //pressed Enter
							Tab::current->file->insert( Tab::current->file->cursorPos.y + 1 , "" ); //add new line to file
							Tab::current->file->at( Tab::current->file->cursorPos.y + 1 ) += Tab::current->file->at( Tab::current->file->cursorPos.y ).substr( Tab::current->file->cursorPos.x , Tab::current->file->at( Tab::current->file->cursorPos.y ).length() - Tab::current->file->cursorPos.x ); //add segment to next line
							Tab::current->file->getCurrentLine() = Tab::current->file->at( Tab::current->file->cursorPos.y ).substr( 0 , Tab::current->file->cursorPos.x ); //delete leftovers from old line

							Tab::current->file->cursorPos.x = 0; //move cursor to beginning of next row
							Tab::current->file->cursorPos.y++; //move cursor to next line

							Tab::current->file->addTabSpace();

							/* === Add Closing Brace If Needed === */
							if ( Tab::current->file->bracketMatch > 0 ) {
								Tab::current->file->insert( Tab::current->file->cursorPos.y + 1 , "" ); //add line for bracket
								Tab::current->file->cursorPos.x = 0;
								Tab::current->file->cursorPos.y++;

								Tab::current->file->tabSpaceCount--;
								Tab::current->file->addTabSpace();
								Tab::current->file->tabSpaceCount++;

								Tab::current->file->insert( "}" );
								Tab::current->file->bracketMatch--;

								Tab::current->file->cursorPos.x = Tab::current->file->getCurrentLine().length();
								Tab::current->file->cursorPos.y--;
							}
							/* =================================== */
						}

						else if ( !pressedControl() && (event.text.unicode >= 32 || event.text.unicode == 9) ) {
							if ( event.text.unicode == 123 ) { // pressed {
								Tab::current->file->bracketMatch++;
								if ( Tab::current->file->bracketMatch > 0 ) // since some {'s were deleted, it should be replaced without shifting right
									Tab::current->file->tabSpaceCount++;
							}

							else if ( event.text.unicode == 125 ) { // pressed }
								if ( Tab::current->file->bracketMatch > 0 ) {
									Tab::current->file->bracketMatch--;
									Tab::current->file->tabSpaceCount--;
								}
							}

							Tab::current->file->insert( event.text.unicode );
						}
					}

					else if ( pressedControl() && ( (pressedShift() && keyPressed( sf::Keyboard::Tab )) || keyPressed( sf::Keyboard::PageUp ) ) ) { //pressed Ctrl + Shift + Tab OR Ctrl + PageUp
						if ( Tab::tabIndex > 0 ) {
							Tab::tabIndex--;
							Tab::current = Tab::tabsOpen[Tab::tabIndex];
						}

						else { // loop around to back of tab list
							Tab::tabIndex = Tab::tabsOpen.size() - 1;
							Tab::current = Tab::tabsOpen[Tab::tabIndex];
						}
					}

					else if ( pressedControl() && ( keyPressed( sf::Keyboard::Tab ) || keyPressed( sf::Keyboard::PageDown ) ) ) { //pressed Ctrl + Tab OR Ctrl + PageDown
						if ( Tab::tabIndex < Tab::tabsOpen.size() - 1 ) {
							Tab::tabIndex++;
							Tab::current = Tab::tabsOpen[Tab::tabIndex];
						}

						else { // loop around to front of tab list
							Tab::tabIndex = 0;
							Tab::current = Tab::tabsOpen[0];
						}
					}

					else if ( keyPressed( sf::Keyboard::Home ) )
						Tab::current->file->cursorPos.x = 0;

					else if ( keyPressed( sf::Keyboard::End ) )
						Tab::current->file->cursorPos.x = Tab::current->file->getCurrentLine().length();

					/* ===== Execute hotKeys that have no button ===== */
					else if ( pressedControl() && keyPressed( sf::Keyboard::A ) ) { //pressed Ctrl + A
						Tab::current->file->allSelected = true;
					}

					else if ( pressedControl() && keyPressed( sf::Keyboard::W ) ) { //pressed Ctrl + W
						Tab::current->closeTab();
					}
					/* =============================================== */

					else if ( keyPressed( sf::Keyboard::Right ) ) {
						// adjust tab spacing when brackets are entered or left
						if ( Tab::current->file->getCurrentCharacters( 1 , 0 ) == "{" )
							Tab::current->file->tabSpaceCount++;
						else if ( Tab::current->file->getCurrentCharacters( 1 , 0 ) == "}" )
							Tab::current->file->tabSpaceCount--;

						if ( Tab::current->file->cursorPos.x < Tab::current->file->at( Tab::current->file->cursorPos.y ).length() ) //if cursor isn't at end of row, move it normally
							Tab::current->file->cursorPos.x++;

						else if ( Tab::current->file->cursorPos.y < static_cast<short>(Tab::current->file->size() - 1) ) { //if there is another row beneath and the cursor is at end or row, move cursor to beginning of next row
							Tab::current->file->cursorPos.x = 0;
							Tab::current->file->cursorPos.y++;
						}
					}

					else if ( keyPressed( sf::Keyboard::Left ) ) {
						// adjust tab spacing when brackets are entered or left
						if ( Tab::current->file->getCurrentCharacters( 1 , 0 ) == "{" )
							Tab::current->file->tabSpaceCount--;
						else if ( Tab::current->file->getCurrentCharacters( 1 , 0 ) == "}" )
							Tab::current->file->tabSpaceCount++;

						if ( Tab::current->file->cursorPos.x > 0 )
							Tab::current->file->cursorPos.x--;

						else if ( Tab::current->file->cursorPos.y > 0 && Tab::current->file->cursorPos.x == 0 ) { //if there is a row above and cursor is at beginning of row, move cursor to end of previous row
							Tab::current->file->cursorPos.y--;
							Tab::current->file->cursorPos.x = Tab::current->file->at( Tab::current->file->cursorPos.y ).length();
						}
					}

					else if ( keyPressed( sf::Keyboard::Down ) ) {
						if ( Tab::current->file->cursorPos.y < Tab::current->file->size() - 1 ) {
							Tab::current->file->cursorPos.y++;

							if ( static_cast<unsigned int>(Tab::current->file->cursorPos.x) > Tab::current->file->getCurrentLine().length() )
								Tab::current->file->cursorPos.x = Tab::current->file->getCurrentLine().length();
						}
					}

					else if ( keyPressed( sf::Keyboard::Up ) ) {
						if ( Tab::current->file->cursorPos.y > 0 )
							Tab::current->file->cursorPos.y--;

						if ( static_cast<unsigned int>(Tab::current->file->cursorPos.x) > Tab::current->file->getCurrentLine().length() )
							Tab::current->file->cursorPos.x = Tab::current->file->getCurrentLine().length();
					}
#endif

					globalMutex.unlock();
				}

				for ( unsigned int index = 0 ; index < Button::allButtons.size() ; index++ ) { // handles all hotkeys that are assigned to buttons
					if ( Button::allButtons[index]->hotKeyActivated() ) {
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

							if ( DropDown::currentOpen != NULL && keyPressed( sf::Keyboard::Escape ) ) //if escape pressed, closes menu
								DropDown::currentOpen->closeMenu();
						}
					}
				}
				/* =================================================== */

				drawObjects();
			}

			drawObjects();
		}

		sf::sleep( sf::Time( 1000 ) );
	}

	// Clean up window
	DestroyWindow( Window );
	UnregisterClass( "AvianCarrier" , Instance );

	fclose( stderr );

	return EXIT_SUCCESS;
}

bool SetCurrentDirectoryA( std::string dir ) {
	return SetCurrentDirectoryA( dir.c_str() );
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
		if ( getList( searchDir + "\\" + name ) != NULL )
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
	globalMutex.unlock();
	CLOSE_THREADS = true;

	mainWin.close();
}

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
								std::string temp = searchDir + "\\" + (*fileList)[selection].getString();

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
									Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , 50001 , (*fileList)[selection].getString() );
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
					std::string temp = searchDir + "\\" + (*fileList)[selection].getString();

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
						Tab::newTab( sf::IpAddress( 127 , 0 , 0 , 1 ) , 50001 , (*fileList)[selection].getString() );
						openBox.close();
					}
				}
			}

			if ( (Cancel.isHovered( openBox ) && mouseButtonReleased( event , sf::Mouse::Left )) || keyPressed( sf::Keyboard::Escape ) )
				openBox.close();

			if ( newEvent && ( (prevDir.isHovered( openBox ) && mouseButtonReleased( event , sf::Mouse::Left )) || keyReleased( event , sf::Keyboard::Back ) ) ) {
				if ( searchDir != rootDirectory + "\\Documents" ) {
					delete fileList;
					searchDir = searchDir = searchDir.substr( 0 , searchDir.rfind( "\\" ) ); // update search directory to parent folder
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

	searchDir = rootDirectory + "\\Documents"; // reset starting directory in open dialog

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
	AllocConsole(); // create console for GCC output
	statusBar.setStatus( "Building..." );
	mainWin.draw( statusBar );
	mainWin.display();
	std::cout << "Tab::current->file->fileName=" << Tab::current->file->fileName << "\n";
	if ( !sysRun( "C:/Users/Tyler/Downloads/EclipseCDT/mingw/bin/g++.exe" , "-O3 -Os -Wall -c -fmessage-length=0 -std=c++0x -o " + searchDir + "\\" + changeExtension( Tab::current->file->fileName , "o" ) + " " + searchDir + "\\" + changeExtension( Tab::current->file->fileName , "cpp" ) ) )
		sysRun( "C:/Users/Tyler/Downloads/EclipseCDT/mingw/bin/g++.exe" , "-s -static -o " + searchDir + "\\" + changeExtension( Tab::current->file->fileName , "exe" ) + " " + searchDir + "\\" + changeExtension( Tab::current->file->fileName , "o" ) );

	statusBar.setStatus( "Done" );
}

void startChat() {
	if ( chatDialog.isVisible() ) { // close chat window
		chatDialog.setVisible( false );
		mainWin.setSize( sf::Vector2u( mainWin.getSize().x - chatDialog.getSize().x , mainWin.getSize().y ) );
	}
	else { // open chat window
		chatDialog.setVisible( true );
		mainWin.setSize( sf::Vector2u( mainWin.getSize().x + chatDialog.getSize().x , mainWin.getSize().y ) );
	}
}

void displayAbout() {
	sf::SplashScreen splash( 400 , 250 , "" , sf::Style::Titlebar , "Pigeot Splash.png" );

	splash.waitForExitClick();
}

int sysRun( std::string programAndDir , std::string args ) {
	std::cout << "{" << programAndDir << " " << args << "}\n";

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	// Start the child process.
	if( !CreateProcess( programAndDir.c_str(),   // No module name (use command line)
		const_cast<char*>( args.c_str() ),   // Command line
		NULL,                                // Process handle not inheritable
		NULL,                                // Thread handle not inheritable
		FALSE,                               // Set handle inheritance to FALSE
		0,                                   // No creation flags
		NULL,                                // Use parent's environment block
		NULL,                                // Use parent's starting directory
		&si,                                 // Pointer to STARTUPINFO structure
		&pi )                                // Pointer to PROCESS_INFORMATION structure
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
