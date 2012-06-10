//=============================================================================
//File Name: RenderFile.cpp
//Description: Holds definitions for RenderFile derived class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include "GUI/RenderFile.h"
#include <SFML/Graphics/Text.hpp>
#include "Base.h"

RenderFile::RenderFile( sf::IpAddress address , unsigned short port , std::string path ) : File( address , port , path ) {
	lineRenderStart = 0;
}

RenderFile::~RenderFile() {

}

void RenderFile::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	sf::Text drawMe( "" , Base::consolas , 12 );
	drawMe.setColor( sf::Color( 0 , 0 , 0 ) );

	for ( int drawIndex = lineRenderStart , lineIndex = lineRenderStart ; lineIndex < static_cast<int>(input.size()) ; lineIndex++ , drawIndex++ ) {
		drawMe.setString( input.at( lineIndex ) );
		drawMe.setPosition( 5 , 60 + (drawIndex - lineRenderStart) * drawMe.getCharacterSize() );
		target.draw( drawMe , states );
	}

	/* ===== Draw cursor ===== */
	drawMe.setString( "_" );

	unsigned int xPos = 0;
	for ( unsigned int index = 0 ; index < cursorPos.x ; index++ ) {
		if ( input.at(cursorPos.y).substr( index , 1 ) == "\t" )
			xPos += 4;
		else
			xPos++;
	}
	drawMe.setPosition( 5 + 7 * xPos , 60 + 12 * (cursorPos.y - lineRenderStart) );
	target.draw( drawMe , states );
	/* ======================= */
}
