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
