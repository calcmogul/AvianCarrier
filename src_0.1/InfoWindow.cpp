//=============================================================================
//File Name: InfoWindow.cpp
//Description: Holds definitions for InfoWindow class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "GUI/InfoWindow.h"
#include "Base.h"

namespace sf {

InfoWindow::InfoWindow( const char* lpWindowName,
			int nWidth,
			int nHeight,
			sf::RenderWindow* parentWin,
			sf::Uint32 style ,
			sf::Image* icon ,
			std::string shaderFile ,
			std::string shaderType ) : sfWindow() {
	parentWindow = parentWin;

	if ( icon != NULL )
		sfWindow.setIcon( icon->getSize().x , icon->getSize().y , icon->getPixelsPtr() );

	bool loaded = false;
	if ( shaderFile != "" ) {
		if ( shaderType == "frag" )
			loaded += shader.loadFromFile( "Resource/" + shaderFile + "." + shaderType , sf::Shader::Fragment );
		else if ( shaderType == "vert" )
			loaded += shader.loadFromFile( "Resource/" + shaderFile + "." + shaderType , sf::Shader::Vertex );
		else
			throw "Unidentified shader type; must be fragment or vertex shader (\"frag\" or \"vert\")";

		if ( shader.isAvailable() )
			shaderState.shader = &shader;
		else
			shaderState = sf::RenderStates::Default;
	}

	sfWindow.create( sf::VideoMode( nWidth , nHeight ) , lpWindowName , style );
	sfWindow.setPosition( sf::Vector2i( parentWindow->getPosition().x + ( parentWindow->getSize().x - nWidth ) / 2 , parentWindow->getPosition().y + ( parentWindow->getSize().y - nHeight ) / 2 ) );
}

InfoWindow::~InfoWindow() {
	if ( sfWindow.isOpen() )
		sfWindow.close();
	parentWindow->setActive( true );
}

void InfoWindow::clear( const Color& color ) {
	sf::RectangleShape background( sf::Vector2f( sfWindow.getSize().x , sfWindow.getSize().y ) );
	sfWindow.clear( color );
	sfWindow.display();

	background.setFillColor( color );

	if ( shader.isAvailable() ) {
		setShaderParam( "width" , sfWindow.getSize().x );
		setShaderParam( "height" , sfWindow.getSize().y );
	}

	sfWindow.draw( background , shaderState );
}

void InfoWindow::print( std::string message , short x , short y , unsigned int size , const Color& color ) {
	sf::Text input( message , Base::segoeUI , size );
	input.setColor( color );

	if ( x == -1 )
		x = ( sfWindow.getSize().x - input.findCharacterPos( message.length() ).x ) / 2.f;

	if ( y == -1 )
		y = ( sfWindow.getSize().y - input.getCharacterSize() ) / 2.f;

	input.setPosition( x , y );

	sfWindow.draw( input );
}

void InfoWindow::setTitle( const std::string& title ) {
	sfWindow.setTitle( title );
}

void InfoWindow::display() {
	sfWindow.display();
}

void InfoWindow::setPosition( Vector2i position ) {
	sfWindow.setPosition( position );
}

bool InfoWindow::checkEvent() {
	if ( sfWindow.pollEvent( event ) )
		return true;
	else
		return false;
}

void InfoWindow::waitForExit() {
	while ( sfWindow.isOpen() ) {
		if ( sf::Mouse::isButtonPressed( sf::Mouse::Left ) )
			sfWindow.close();
	}
}

}
