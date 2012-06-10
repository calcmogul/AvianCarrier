//=============================================================================
//File Name: DropDown.h
//Description: Holds definitions of Button GUI class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include <SFML/Graphics/RenderTarget.hpp>
#include "Base.h"
#include "GUI/Button.h"

const unsigned char Button::colorXOR = BUTTON_HOVER ^ BUTTON_OFF;
std::vector<Button*> Button::allButtons;

std::map<std::string , sf::Keyboard::Key> Button::hotKeyConverter;
bool Button::isLoaded = false;

Button::Button( std::string message , std::string note , std::string hotkeys , short textAlign , unsigned char borderColor , void (*operation)() , bool use ) : sfText( message , Base::segoeUI , 12 ) , sfText2( note , Base::segoeUI , 12 ) {
	if ( !isLoaded ) {
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "A" , sf::Keyboard::A ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "B" , sf::Keyboard::B ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "C" , sf::Keyboard::C ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "D" , sf::Keyboard::D ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "E" , sf::Keyboard::E ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F" , sf::Keyboard::F ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "G" , sf::Keyboard::G ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "H" , sf::Keyboard::H ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "I" , sf::Keyboard::I ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "J" , sf::Keyboard::J ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "K" , sf::Keyboard::K ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "L" , sf::Keyboard::L ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "M" , sf::Keyboard::M ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "N" , sf::Keyboard::N ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "O" , sf::Keyboard::O ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "P" , sf::Keyboard::P ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Q" , sf::Keyboard::Q ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "R" , sf::Keyboard::R ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "S" , sf::Keyboard::S ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "T" , sf::Keyboard::T ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "U" , sf::Keyboard::U ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "V" , sf::Keyboard::V ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "W" , sf::Keyboard::W ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "X" , sf::Keyboard::X ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Y" , sf::Keyboard::Y ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Z" , sf::Keyboard::Z ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num0" , sf::Keyboard::Num0 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num1" , sf::Keyboard::Num1 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num2" , sf::Keyboard::Num2 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num3" , sf::Keyboard::Num3 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num4" , sf::Keyboard::Num4 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num5" , sf::Keyboard::Num5 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num6" , sf::Keyboard::Num6 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num7" , sf::Keyboard::Num7 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num8" , sf::Keyboard::Num8 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Num9" , sf::Keyboard::Num9 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Escape" , sf::Keyboard::Escape ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "LControl" , sf::Keyboard::LControl ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "LShift" , sf::Keyboard::LShift ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "LAlt" , sf::Keyboard::LAlt ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "LSystem" , sf::Keyboard::LSystem ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "RControl" , sf::Keyboard::RControl ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "RShift" , sf::Keyboard::RShift ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "RAlt" , sf::Keyboard::RAlt ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "RSystem" , sf::Keyboard::RSystem ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Menu" , sf::Keyboard::Menu ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "LBracket" , sf::Keyboard::LBracket ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "RBracket" , sf::Keyboard::RBracket ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "SemiColon" , sf::Keyboard::SemiColon ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Comma" , sf::Keyboard::Comma ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Period" , sf::Keyboard::Period ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Quote" , sf::Keyboard::Quote ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Slash" , sf::Keyboard::Slash ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "BackSlash" , sf::Keyboard::BackSlash ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Tilde" , sf::Keyboard::Tilde ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Equal" , sf::Keyboard::Equal ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Dash" , sf::Keyboard::Dash ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Space" , sf::Keyboard::Space ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Return" , sf::Keyboard::Return ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Back" , sf::Keyboard::Back ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Tab" , sf::Keyboard::Tab ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "PageUp" , sf::Keyboard::PageUp ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "PageDown" , sf::Keyboard::PageDown ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "End" , sf::Keyboard::End ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Home" , sf::Keyboard::Home ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Insert" , sf::Keyboard::Insert ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Delete" , sf::Keyboard::Delete ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Add" , sf::Keyboard::Add ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Subtract" , sf::Keyboard::Subtract ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Multiply" , sf::Keyboard::Multiply ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Divide" , sf::Keyboard::Divide ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Left" , sf::Keyboard::Left ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Right" , sf::Keyboard::Right ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Up" , sf::Keyboard::Up ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Down" , sf::Keyboard::Down ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad0" , sf::Keyboard::Numpad0 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad1" , sf::Keyboard::Numpad1 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad2" , sf::Keyboard::Numpad2 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad3" , sf::Keyboard::Numpad3 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad4" , sf::Keyboard::Numpad4 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad5" , sf::Keyboard::Numpad5 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad6" , sf::Keyboard::Numpad6 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad7" , sf::Keyboard::Numpad7 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad8" , sf::Keyboard::Numpad8 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Numpad9" , sf::Keyboard::Numpad9 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F1" , sf::Keyboard::F1 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F2" , sf::Keyboard::F2 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F3" , sf::Keyboard::F3 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F4" , sf::Keyboard::F4 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F5" , sf::Keyboard::F5 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F6" , sf::Keyboard::F6 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F7" , sf::Keyboard::F7 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F8" , sf::Keyboard::F8 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F9" , sf::Keyboard::F9 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F10" , sf::Keyboard::F10 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F11" , sf::Keyboard::F11 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F12" , sf::Keyboard::F12 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F13" , sf::Keyboard::F13 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F14" , sf::Keyboard::F14 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "F15" , sf::Keyboard::F15 ) );
		hotKeyConverter.insert( std::pair<std::string , sf::Keyboard::Key>( "Pause" , sf::Keyboard::Pause ) );

		isLoaded = true;
	}

	allButtons.push_back( this );

	text = message;
	text2 = note;

	// Set up HotKey handler
	for ( unsigned int index = 0 ; index < hotkeys.length() ; index++ ) {
		if ( hotkeys.substr( index , hotkeys.find( " " , index ) ) == "Control" )
			hotKeyList.push_back( sf::Keyboard::LControl );
		else if ( hotkeys.substr( index , hotkeys.find( " " , index ) ) == "Shift" )
			hotKeyList.push_back( sf::Keyboard::LShift );
		else if ( hotkeys.substr( index , hotkeys.find( " " , index ) ) == "Alt" )
			hotKeyList.push_back( sf::Keyboard::LAlt );
		else
			hotKeyList.push_back( hotKeyConverter[ hotkeys.substr( index , hotkeys.find( " " , index ) - index ) ] );

		index = hotkeys.find( " " , index );
		if ( index > hotkeys.length() )
			index--;
	}

	alignment = textAlign;

	useable = use;
	m_visible = true;
	colorVal = sf::Color( BUTTON_OFF , BUTTON_OFF , BUTTON_OFF );

	setUseable( useable );

	if ( note == "" )
		setSize( sf::Vector2f( sfText.findCharacterPos( text.length() ).x + BUTTON_SPACING , 20 ) );
	else
		setSize( sf::Vector2f( 150 , 20 ) );

	setOutlineThickness( 1.f );
	setOutlineColor( sf::Color( borderColor , borderColor , borderColor ) );
	setFillColor( sf::Color( BUTTON_OFF , BUTTON_OFF , BUTTON_OFF ) );

	func = operation;
}

Button::~Button() {
	std::vector<Button*>::iterator index;
	for ( index = allButtons.begin() ; *index != this ; index++ ) {
		if ( index >= allButtons.end() )
			return;
	}

	allButtons.erase( index );
}

void Button::setPosition( const sf::Vector2f& position ) {
	Clickable::setPosition( position );

	if ( alignment == -1 )
		recenterText();
	else
		sfText.setPosition( getPosition().x + alignment , getPosition().y + ( 20.f - sfText.getCharacterSize() ) / 2 - 1 );
}

void Button::setPosition( float x , float y ) {
	Clickable::setPosition( x , y );

	if ( alignment == -1 )
		recenterText();
	else
		sfText.setPosition( getPosition().x + alignment , getPosition().y + ( 20.f - sfText.getCharacterSize() ) / 2 - 1 );
}

void Button::recenterText() {
	if ( text2 != "" ) {
		sfText.setPosition( getPosition().x + BUTTON_SPACING / 2 ,getPosition().y + ( 20.f - sfText.getCharacterSize() ) / 2 - 1 );
		sfText2.setPosition( getPosition().x + getSize().x - sfText2.findCharacterPos( text2.length() ).x + sfText2.getPosition().x - BUTTON_SPACING / 2 , getPosition().y + ( 20.f - sfText2.getCharacterSize() ) / 2 - 1 );
	}
	else
		sfText.setPosition( getPosition().x + ( getSize().x - sfText.findCharacterPos( text.length() ).x + sfText.getPosition().x ) / 2 , getPosition().y + ( 20.f - sfText.getCharacterSize() ) / 2 - 1 );
}

const std::string& Button::getText() {
	return text;
}

const std::string& Button::getText2() {
	return text2;
}

void Button::setFillColor( sf::Color fill ) {
	colorVal = fill;

	Clickable::setFillColor( fill );
}

void Button::setFillColor( unsigned char fill ) {
	colorVal = sf::Color( fill , fill ,fill );

	Clickable::setFillColor( colorVal );
}

sf::Color Button::getFillColor() {
	return colorVal;
}

void Button::setUseable( bool use ) {
	useable = use;

	if ( useable ) { // dims options that aren't available AKA not useable
		sfText.setColor( sf::Color( 255 , 255 , 255 ) );

		if ( text2 != "" )
			sfText2.setColor( sf::Color( 255 , 255 , 255 ) );
	}
	else {
		sfText.setColor( sf::Color( 130 , 130 , 130 ) );

		if ( text2 != "" )
			sfText2.setColor( sf::Color( 130 , 130 , 130 ) );
	}
}

bool Button::isUseable() {
	return useable;
}

void Button::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	if ( m_visible ) {
		target.draw( static_cast<sf::RectangleShape>(*this) , states );
		target.draw( sfText , states );
	}

	if ( text2 != "" )
		target.draw( sfText2 , states );
}

bool Button::hotKeyActivated( sf::Event& event ) {
	if ( useable && hotKeyList.size() > 0 ) {
		bool activated = true;

		for ( unsigned int index = 0 ; activated && index < hotKeyList.size() ; index++ ) {
			if ( !sf::Keyboard::isKeyPressed( hotKeyList[index] ) ) {
				if ( hotKeyList[index] == sf::Keyboard::LControl || hotKeyList[index] == sf::Keyboard::RControl ) {
					if ( !pressedControl( event ) )
						activated = false;
				}

				else if ( hotKeyList[index] == sf::Keyboard::LShift || hotKeyList[index] == sf::Keyboard::RShift ) {
					if ( !pressedShift( event ) )
						activated = false;
				}

				else if ( hotKeyList[index] == sf::Keyboard::LAlt || hotKeyList[index] == sf::Keyboard::RAlt ) {
					if ( !pressedAlt( event ) )
						activated = false;
				}

				else
					activated = false;
			}
		}

		/* ===== Handle selecting item in menu : HOTKEYS ===== */
		if ( !hotKeyDelay.updateMove( activated , 0 ) )
			activated = false;
		/* =================================================== */

		return activated;
	}
	else
		return false;
}
