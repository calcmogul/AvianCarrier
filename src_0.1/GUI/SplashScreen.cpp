//=============================================================================
//File Name: SplashScreen.cpp
//Description: Holds definitions for SplashScreen class
//Author: Tyler Veness
//=============================================================================

#include "SplashScreen.h"
#include "../Base.h"
#include <SFML/Window/Mouse.hpp>

namespace sf {

SplashScreen::SplashScreen( const std::string& imageName ) : RenderWindow() {
	if ( imageName != "" ) {
		backTexture.loadFromFile( "Resource/" + imageName );
		backDraw.setTexture( backTexture );
		backDraw.setPosition( Vector2f( 0.f , 0.f ) );
	}
}

SplashScreen::SplashScreen( unsigned int modeX , unsigned int modeY , const std::string& title , Uint32 style , const std::string& imageName ) : RenderWindow( VideoMode( modeX , modeY ) , title , style , ContextSettings() ) {
	if ( imageName != "" ) {
		backTexture.loadFromFile( "Resource/" + imageName );
		backDraw.setTexture( backTexture );
		backDraw.setPosition( Vector2f( 0.f , 0.f ) );
	}
	else {
		backTexture.create( modeX , modeY );
		backDraw.setTexture( backTexture );
	}

	drawAll();
}

SplashScreen::~SplashScreen() {

}

void SplashScreen::create( unsigned int modeX , unsigned int modeY , const std::string& title , Uint32 style , const std::string& imageName ) {
	if ( imageName != "" ) {
		backTexture.loadFromFile( "Resource/" + imageName );
		backDraw.setTexture( backTexture );
		backDraw.setPosition( Vector2f( 0.f , 0.f ) );
	}
	else {
		backTexture.create( modeX , modeY );
		backDraw.setTexture( backTexture );
	}

	Window::create( VideoMode( modeX , modeY ) , title , style );
}

void SplashScreen::drawAll() {
	draw( backDraw );
}

void SplashScreen::waitForExitClick() {
	while ( isOpen() ) {
		if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
			close();
	}
}

}
