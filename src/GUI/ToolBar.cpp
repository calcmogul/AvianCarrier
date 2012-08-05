//=============================================================================
//File Name: ToolBar.cpp
//Description: Holds definitions of Toolbar GUI class
//Author: Tyler Veness
//=============================================================================

#include <SFML/Graphics/RenderTarget.hpp>
#include "Toolbar.h"

ToolBar::ToolBar( float x , float y , float width , float height , std::vector<DropDown*> bar ) : Clickable( sf::Vector2f( width , height ) ) , backgroundEdge( sf::Vector2f( width , height ) ) , tools( bar ) {
	yPos = y;

	backgroundEdge.setPosition( x , y );
	backgroundEdge.setOutlineThickness( 2.f );
	backgroundEdge.setOutlineColor( sf::Color( 45 , 45 , 45 ) );

	setPosition( x , y );
	setOutlineThickness( 1.f );
	setFillColor( sf::Color( 100 , 100 , 100 ) );
	setOutlineColor( sf::Color( 30 , 30 , 30 ) );
}

void ToolBar::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	target.draw( backgroundEdge , states );
	target.draw( static_cast<sf::RectangleShape>(*this) , states );

	for ( unsigned int index = 0 , position = TAB_START_X ; index < tools.size() ; index++ ) {
		tools[index]->setPosition( position , getPosition().y + 2 );

		tools[index]->menu[0]->setPosition( position , yPos + 2 );
		target.draw( *tools[index]->menu[0] , states );

		position += tools[index]->menu[0]->getSize().x + TAB_SPACING;
	}
}

void ToolBar::setVisible( bool visible ) {
	for ( unsigned int index = 0 ; index < tools.size() ; index++ )
		tools[index]->menu[0]->setVisible( visible );
}

void ToolBar::updateSize( sf::Window& target ) {
	backgroundEdge.setSize( sf::Vector2f( target.getSize().x , 24 ) );
	setSize( sf::Vector2f( target.getSize().x , 24 ) );
}
