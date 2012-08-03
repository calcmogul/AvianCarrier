//=============================================================================
//File Name: StatusBar.cpp
//Description: Holds definitions for StatusBar class
//Author: Tyler Veness
//=============================================================================

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>
#include "StatusBar.h"
#include "../Base.h"

StatusBar::StatusBar() : messageBuffer( "Done" , Base::segoeUI , 13 ) {
	statusBackground.pushEdge( sf::Vector2f() , sf::Color( 45 , 45 , 45 ) );
	statusBackground.setOutlineThickness( 1.f );
	statusBackground.setFillColor( sf::Color( 100 , 100, 100 ) );
	statusBackground.setOutlineColor( sf::Color( 30 , 30 , 30 ) );

	messageBuffer.setString( "Done" );
}

StatusBar::~StatusBar() {

}

void StatusBar::updateSize( sf::Window& target ) {
	statusBackground.setSize( sf::Vector2f( target.getSize().x , 16.f ) );
	statusBackground.setPosition( sf::Vector2f( 0 , target.getSize().y - statusBackground.getSize().y ) );

	messageBuffer.setPosition( sf::Vector2f( 2 , statusBackground.getPosition().y - 1 ) );
}

std::string StatusBar::getStatus() {
	return messageBuffer.getString();
}

void StatusBar::setStatus( std::string message ) {
	if ( message != "" )
		messageBuffer.setString( message );
}

const sf::Vector2f& StatusBar::getSize() {
	return statusBackground.getSize();
}

const sf::Vector2f& StatusBar::getPosition() {
	return statusBackground.getPosition();
}

void StatusBar::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	target.draw( statusBackground );
	target.draw( messageBuffer );
}
