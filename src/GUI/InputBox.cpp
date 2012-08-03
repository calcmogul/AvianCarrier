//=============================================================================
//File Name: InputBox.cpp
//Description: Holds definitions for InputBox class
//Author: Tyler Veness
//=============================================================================

#include "../Base.h"
#include "InputBox.h"

InputBox::InputBox( const sf::Vector2f& size , std::vector<sf::Color> colors ) : TextReceiver( sf::Vector2f( size ) , colors ) , drawInput( "" , Base::segoeUI , 12 ) {
	setOutlineThickness( 1.f );
	setFillColor( sf::Color( 255 , 255 , 255 ) );
	setOutlineColor( sf::Color( 100 , 100 , 100 ) );

	cursorPos = 0;
	drawInput.setColor( sf::Color( 0 , 0 , 0 ) );
}

InputBox::~InputBox() {

}

void InputBox::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	if ( m_visible ) {
		target.draw( static_cast<sf::RectangleShape>(*this) , states );
		target.draw( drawInput , states );

		sf::Text subText( "" , Base::segoeUI , 12 ); // helps alignment of cursor within text being editted

		sf::Text cursor( "_" , Base::segoeUI , 12 );
		cursor.setColor( sf::Color( 0 , 0 , 0 ) );

		unsigned int currentLine = 0;
		unsigned int length = 0;

		while ( length < std::string(drawInput.getString()).length() ) {

			unsigned int index = 0;
			while ( drawInput.findCharacterPos( index + 1 ).x < getSize().x && std::string(drawInput.getString()).substr( index , 1 ) != "\n" && index < std::string(drawInput.getString()).length() ) {
				if ( index < std::string(drawInput.getString()).length() - 1 ) {
					subText.setString( std::string(drawInput.getString()).substr( length , index ) );
					subText.setPosition( sf::Vector2f( 1.f , currentLine * subText.getCharacterSize() ) );

					if ( cursorPos >= length && cursorPos <= length + index ) {
						cursor.setPosition( sf::Vector2f( subText.findCharacterPos( index ).x , currentLine * subText.getCharacterSize() ) );
						target.draw( cursor , states );
					}

					currentLine++;
				}

				index++;
			}

			length += index;
		}
	}
}

void InputBox::handleEvent( sf::Event& event ) {
	if ( event.type == sf::Event::TextEntered ) {
		if ( event.text.unicode == 8 ) { // pressed Backspace
			if ( cursorPos > 0 ) { // and cursor isn't at beginning of doc
				input = input.substr( 0 , cursorPos - 1 ) + input.substr( cursorPos , input.length() - cursorPos );
				cursorPos--;
				drawInput.setString( input );
			}
		}

		else if ( pressedShift( event ) && event.text.unicode == 10 ) { // pressed Shift + Enter
			input.insert( cursorPos , "\n" );

			cursorPos++;
		}

		else if ( event.text.unicode == 10 ) // pressed Enter
			sendToIP();

		else if ( !pressedControl( event ) && (event.text.unicode >= 32 || event.text.unicode == 9) ) {
			std::string temp;
			temp += event.text.unicode;
			input.insert( cursorPos , temp );
			drawInput.setString( input );

			cursorPos++;
		}
	}

	if ( keyDelay.updateMove( keyPressed( event , sf::Keyboard::Right ) , 0 ) ) {
		if ( cursorPos < input.length() ) // if cursor isn't at end of row, move it normally
			cursorPos++;
	}

	else if ( keyDelay.updateMove( keyPressed( event , sf::Keyboard::Left ) , 0 ) ) {
		if ( cursorPos > 0 )
			cursorPos--;
	}

	else if ( keyPressed( event , sf::Keyboard::Down ) ) {

	}

	else if ( keyPressed( event , sf::Keyboard::Up ) ) {

	}
}

void InputBox::setPosition( const sf::Vector2f& position ) {
	Clickable::setPosition( position );
	drawInput.setPosition( 1.f + getPosition().x , 1.f + getPosition().y );
}

void InputBox::sendToIP() {

}

void InputBox::receiveFromAny() {

}
