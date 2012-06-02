//=============================================================================
//File Name: TextReceiver.cpp
//Description: Holds definitions for TextReceiver class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include "TextReceiver.h"
#include <iostream>//FIXME

std::vector<TextReceiver*> TextReceiver::textInterfaces;
TextReceiver* TextReceiver::currentReceiver = NULL;

TextReceiver::TextReceiver( const sf::Vector2f& size , std::vector<sf::Color> borders ) : Clickable( size , borders ) {
	textInterfaces.push_back( this );
	std::cout << "Added interface " << textInterfaces.size() << "\n";
}

TextReceiver::~TextReceiver() {
	std::cout << "Destructing from size " << textInterfaces.size() << "\n";
	std::vector<TextReceiver*>::iterator index;
	for ( index = textInterfaces.begin() ; *index != this ; index++ ) {
		if ( index >= textInterfaces.end() )
			return;
	}

	if ( currentReceiver == this )
		currentReceiver = NULL;

	textInterfaces.erase( index );
}

void TextReceiver::checkSwitchReceiver( sf::Window& referTo ) {
	std::cout << "textInterfaces.size()=" << textInterfaces.size() << "\n";
	for ( unsigned int index = 0 ; index < textInterfaces.size() ; index++ ) {
		if ( textInterfaces[index]->isClicked( referTo ) && currentReceiver != textInterfaces[index] )
			currentReceiver = textInterfaces[index];
	}
}

/*const TextReceiver& TextReceiver::getReceiver() {
	return *currentReceiver;
}*/

void TextReceiver::setReceiver( TextReceiver* activated ) {
	currentReceiver = activated;
}

bool TextReceiver::isReceiving() {
	return currentReceiver == this;
}
