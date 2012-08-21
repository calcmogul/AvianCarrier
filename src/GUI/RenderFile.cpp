//=============================================================================
//File Name: RenderFile.cpp
//Description: Holds definitions for RenderFile derived class
//Author: Tyler Veness
//=============================================================================

#include <SFML/Graphics/Text.hpp>
#include "../Base.h"
#include "RenderFile.h"
#include "Tab.h"

RenderFile::RenderFile( sf::IpAddress address , std::string path ) : File( address , path ) {
	lineRenderStart = 0;
}

RenderFile::~RenderFile() {

}

void RenderFile::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	sf::Text drawMe( "" , Base::consolas , 12 );
	drawMe.setColor( sf::Color( 0 , 0 , 0 ) );

	for ( int drawIndex = lineRenderStart , lineIndex = lineRenderStart ; lineIndex < static_cast<int>(input.size()) ; lineIndex++ , drawIndex++ ) {
		drawMe.setString( input.at( lineIndex ) );
		drawMe.setPosition( 5 , Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 2 + (drawIndex - lineRenderStart) * drawMe.getCharacterSize() );
		target.draw( drawMe , states );
	}
}

void RenderFile::drawCursor() {
	sf::Text drawMe( "" , Base::consolas , 12 );
	drawMe.setColor( sf::Color( 0 , 0 , 0 ) );

	/* ===== Draw cursor ===== */
	drawMe.setString( "|" );

	unsigned int xPos = 0;
	fileProtect.startReading();
	for ( unsigned int index = 0 ; index < cursorPos.x ; index++ ) {
		if ( input.at(cursorPos.y).substr( index , 1 ) == "\t" )
			xPos += 4;
		else
			xPos++;
	}
	drawMe.setPosition( 5 + 7 * xPos - 3/* almost half width of character */ , Tab::tabBase.getPosition().y + Tab::tabBase.getSize().y + 2 + 12 * (cursorPos.y - lineRenderStart) );

	fileProtect.stopReading();

	mainWin.draw( drawMe );
	/* ======================= */
}
