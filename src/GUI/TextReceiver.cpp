//=============================================================================
//File Name: TextReceiver.cpp
//Description: Holds definitions for TextReceiver class
//Author: Tyler Veness
//=============================================================================

#include "TextReceiver.h"
#include <SFML/Window/Mouse.hpp>

std::vector<TextReceiver*> TextReceiver::textInterfaces;
TextReceiver* TextReceiver::currentReceiver = NULL;

TextReceiver::TextReceiver( const sf::Vector2f& size , std::vector<sf::Color> borders ) : Clickable( size , borders ) {
	textInterfaces.push_back( this );
	m_active = true;
}

TextReceiver::~TextReceiver() {
	if ( textInterfaces.size() > 0 ) {
		std::vector<TextReceiver*>::iterator index;
		for ( index = textInterfaces.begin() ; *index != this ; index++ ) {
			if ( index >= textInterfaces.end() )
				return;
		}

		if ( currentReceiver == this )
			currentReceiver = NULL;

		textInterfaces.erase( index );
	}
}

void TextReceiver::checkSwitchReceiver( sf::Window& referTo ) {
	bool switched = false;

	for ( unsigned int index = 0 ; index < textInterfaces.size() && !switched ; index++ ) {
		if ( textInterfaces[index]->isClicked( referTo ) ) {
			currentReceiver = textInterfaces[index];
			switched = true;
		}
		else if ( sf::Mouse::isButtonPressed( sf::Mouse::Button::Left ) )
			currentReceiver = NULL;
	}
}

void TextReceiver::setReceiver( TextReceiver* activated ) {
	currentReceiver = activated;
}

bool TextReceiver::isReceiving() {
	return currentReceiver == this;
}

void TextReceiver::activate( bool active ) {
	m_active = active;

	if ( active )
		setFillColor( sf::Color( 255 , 255 , 255 ) );
	else
		setFillColor( sf::Color( 80 , 80 , 80 ) );
}

bool TextReceiver::isActive() {
	return m_active;
}
