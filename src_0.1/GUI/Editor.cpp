//=============================================================================
//File Name: Editor.cpp
//Description: Holds definitions for Editor class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include "../Base.h"
#include "Editor.h"
#include "Tab.h"

Editor::Editor() : TextReceiver( sf::Vector2f( 0.f , 0.f ) ) , vScroll( 15.f , 0.f ) {
	setFillColor( sf::Color( 80 , 80 , 80 ) );
	setVisible( true );
	vScroll.setVisible( true );
}

Editor::Editor( unsigned int x , unsigned int y , unsigned int width , unsigned int height ) : TextReceiver( sf::Vector2f( width , height ) ) , vScroll( 15.f , 0.f ) {
	setFillColor( sf::Color( 80 , 80 , 80 ) );
	setVisible( true );
	vScroll.setVisible( true );
	vScroll.setSize( sf::Vector2f( 15.f , 0.f ) );
}

Editor::~Editor() {

}

void Editor::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	target.draw( static_cast<sf::RectangleShape>(*this) , states );
	Tab::tabMutex.lock();
	if ( Tab::current != NULL )
		target.draw( vScroll , states );
	Tab::tabMutex.unlock();
}

void Editor::handleEvent( sf::Event& event ) {
	Tab::tabMutex.lock();

	if ( Tab::current != NULL ) {
		if ( event.type == sf::Event::TextEntered ) {
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

			else if ( event.text.unicode == 13 ) { //pressed Enter
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

			else if ( !pressedControl( event ) && (event.text.unicode >= 32 || event.text.unicode == 9) ) {
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

		else if ( pressedControl( event ) && ( (pressedShift( event ) && keyPressed( event , sf::Keyboard::Tab )) || keyPressed( event , sf::Keyboard::PageUp ) ) ) { //pressed Ctrl + Shift + Tab OR Ctrl + PageUp
			if ( Tab::tabIndex > 0 ) {
				Tab::tabIndex--;
				Tab::current = Tab::tabsOpen[Tab::tabIndex];
			}

			else { // loop around to back of tab list
				Tab::tabIndex = Tab::tabsOpen.size() - 1;
				Tab::current = Tab::tabsOpen[Tab::tabIndex];
			}
		}

		else if ( pressedControl( event ) && ( keyPressed( event , sf::Keyboard::Tab ) || keyPressed( event , sf::Keyboard::PageDown ) ) ) { //pressed Ctrl + Tab OR Ctrl + PageDown
			if ( Tab::tabIndex < Tab::tabsOpen.size() - 1 ) {
				Tab::tabIndex++;
				Tab::current = Tab::tabsOpen[Tab::tabIndex];
			}

			else { // loop around to front of tab list
				Tab::tabIndex = 0;
				Tab::current = Tab::tabsOpen[0];
			}
		}

		else if ( keyPressed( event , sf::Keyboard::Home ) )
			Tab::current->file->cursorPos.x = 0;

		else if ( keyPressed( event , sf::Keyboard::End ) )
			Tab::current->file->cursorPos.x = Tab::current->file->getCurrentLine().length();

		/* ===== Execute hotKeys that have no button ===== */
		else if ( (sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) || sf::Keyboard::isKeyPressed( sf::Keyboard::RControl )) && sf::Keyboard::isKeyPressed( sf::Keyboard::A ) ) { //pressed Ctrl + A
			Tab::current->file->allSelected = true;
		}

		else if ( (sf::Keyboard::isKeyPressed( sf::Keyboard::LControl ) || sf::Keyboard::isKeyPressed( sf::Keyboard::RControl )) && sf::Keyboard::isKeyPressed( sf::Keyboard::W ) ) { //pressed Ctrl + W
			Tab::current->closeTab();
		}
		/* =============================================== */

		else if ( keyPressed( event , sf::Keyboard::Right ) ) {
			// adjust tab spacing when brackets are entered or left
			if ( Tab::current->file->getCurrentCharacters( 1 , 0 ) == "{" )
				Tab::current->file->tabSpaceCount++;
			else if ( Tab::current->file->getCurrentCharacters( 1 , 0 ) == "}" )
				Tab::current->file->tabSpaceCount--;

			if ( Tab::current->file->cursorPos.x < Tab::current->file->at( Tab::current->file->cursorPos.y ).length() ) //if cursor isn't at end of row, move it normally
				Tab::current->file->cursorPos.x++;

			else if ( Tab::current->file->cursorPos.y < Tab::current->file->size() - 1 ) { //if there is another row beneath and the cursor is at end or row, move cursor to beginning of next row
				Tab::current->file->cursorPos.x = 0;
				Tab::current->file->cursorPos.y++;
			}
		}

		else if ( keyPressed( event , sf::Keyboard::Left ) ) {
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

		else if ( keyPressed( event , sf::Keyboard::Down ) ) {
			if ( Tab::current->file->cursorPos.y < Tab::current->file->size() - 1 ) {
				Tab::current->file->cursorPos.y++;

				if ( static_cast<unsigned int>(Tab::current->file->cursorPos.x) > Tab::current->file->getCurrentLine().length() )
					Tab::current->file->cursorPos.x = Tab::current->file->getCurrentLine().length();
			}
		}

		else if ( keyPressed( event , sf::Keyboard::Up ) ) {
			if ( Tab::current->file->cursorPos.y > 0 )
				Tab::current->file->cursorPos.y--;

			if ( static_cast<unsigned int>(Tab::current->file->cursorPos.x) > Tab::current->file->getCurrentLine().length() )
				Tab::current->file->cursorPos.x = Tab::current->file->getCurrentLine().length();
		}
	}

	Tab::tabMutex.unlock();
}

void Editor::activate( bool active ) {
	if ( active )
		setFillColor( sf::Color( 255 , 255 , 255 ) );
	else
		setFillColor( sf::Color( 80 , 80 , 80 ) );
}

void Editor::setPosition( const sf::Vector2f& position ) {
	Clickable::setPosition( position );
	vScroll.setPosition( getPosition().x + getSize().x - vScroll.getSize().x - 1 , getPosition().y + 1 );
}

void Editor::setSize( const sf::Vector2f& size ) {
	Clickable::setSize( size );
	vScroll.setPosition( getPosition().x + getSize().x - vScroll.getSize().x - 1 , getPosition().y + 1 );
	vScroll.setSize( vScroll.getSize().x , getSize().y - 3 );
}

void Editor::updateSize( sf::Window& target ) {
	setSize( sf::Vector2f( target.getSize().x , target.getSize().y - 16.f - 2.f - (Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 1) ) );
	setPosition( sf::Vector2f( 0 , Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 2 ) );
}
