//=============================================================================
//File Name: Base.cpp
//Description: Holds definitions for Base class
//Author: Tyler Veness
//=============================================================================

#include "Base.h"
#include <sstream>
#include <fstream>

sf::Font Base::consolas;
sf::Font Base::segoeUI;

std::vector<sf::Texture> Base::fileImages;

bool Base::isLoaded = false;

/* ===== STATIC VAR DEFINITIONS ===== */
std::vector<ServerEntry> serverList;
/* ================================== */

bool keyPressed( sf::Event& event , sf::Keyboard::Key key ) {
	return ( event.type == sf::Event::KeyPressed && event.key.code == key );
}

bool keyReleased( sf::Event& event , sf::Keyboard::Key key ) {
	return ( event.type == sf::Event::KeyReleased && event.key.code == key ); // true if key was released and it was the one asked for
}

bool mouseButtonReleased( sf::Event& event , sf::Mouse::Button button ) {
	return ( event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == button ); // true if mouse button was released and it was the one asked for
}

bool pressedControl( sf::Event& event ) {
	return ( keyPressed( event , sf::Keyboard::LControl ) || keyPressed( event , sf::Keyboard::RControl ) );
}

bool pressedShift( sf::Event& event ) {
	return ( keyPressed( event , sf::Keyboard::LShift ) || keyPressed( event , sf::Keyboard::RShift ) );
}

bool pressedAlt( sf::Event& event ) {
	return ( keyPressed( event , sf::Keyboard::LAlt ) || keyPressed( event , sf::Keyboard::RAlt ) );
}

bool mousePressed( sf::Mouse::Button button ) {
	return ( sf::Mouse::isButtonPressed( button ) );
}

Base::Base() {
	if ( !isLoaded ) {
		sf::Image folder;
		sf::Image cFile;
		sf::Image cppFile;
		sf::Image hFile;
		sf::Image hppFile;
		sf::Image txtFile;
		sf::Image unknownFile;

		if ( !consolas.loadFromFile( "Resource/consola.ttf" ) ||
				!segoeUI.loadFromFile( "Resource/segoeui.ttf" ) ||

				!folder.loadFromFile( "Resource/FilePics/folder.png" ) ||
				!cFile.loadFromFile( "Resource/FilePics/cFile.png" ) ||
				!cppFile.loadFromFile( "Resource/FilePics/cppFile.png" ) ||
				!hFile.loadFromFile( "Resource/FilePics/hFile.png" ) ||
				!hppFile.loadFromFile( "Resource/FilePics/hppFile.png" ) ||
				!txtFile.loadFromFile( "Resource/FilePics/txtFile.png" ) ||
				!unknownFile.loadFromFile( "Resource/FilePics/unknownFile.png" ) )
			exit( 1 );
		else {
			// prepare all icons with transparency
			folder.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );
			cFile.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );
			cppFile.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );
			hFile.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );
			hppFile.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );
			txtFile.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );
			unknownFile.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );

			// Load images for file types
			for ( unsigned int index = 0 ; index < 7 ; index++ ) // populate vector with slots for images
				fileImages.push_back( sf::Texture() );

			fileImages[0].loadFromImage( folder );
			fileImages[1].loadFromImage( cFile );
			fileImages[2].loadFromImage( cppFile );
			fileImages[3].loadFromImage( hFile );
			fileImages[4].loadFromImage( hppFile );
			fileImages[5].loadFromImage( txtFile );
			fileImages[6].loadFromImage( unknownFile );

			/* ===== Create list of servers from .ini file ===== */
			std::string line;
			ServerEntry tempEntry;
			std::ifstream servers( "Config/servers.ini" );

			if ( servers.is_open() ) {
				unsigned int index = 0; // increments to 3 then resets to 1 and repeats
				while ( !servers.eof() ) {
					// change what to do with each line extracted
					index %= 3;
					index++;

					std::getline( servers , line );

					if ( index == 1 ) {
						line = line.substr( 0 , line.find( ";" ) ); // remove commented parts of line
						tempEntry.name = line.substr( line.find( "[" ) + 1 , line.find( "]" ) - 1 );
					}

					if ( index == 2 )
						tempEntry.IP = sf::IpAddress( line.substr( line.find( "=" ) + 1 , line.length() - line.find( "=" ) - 1 ) );

					if ( index == 3 ) {
						std::stringstream ss;
						ss << line.substr( line.find( "=" ) , line.length() - line.find( "=" ) );
						unsigned short temp;
						ss >> temp;
						tempEntry.port = temp;

						serverList.push_back( tempEntry );
					}
				}
			}
			/* ================================================= */
		}

		isLoaded = true;
	}
}
