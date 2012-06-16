//=============================================================================
//File Name: ScrollBar.cpp
//Description: Holds definitions for ScrollBar class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include <SFML/Graphics/RenderTarget.hpp>
#include "ScrollBar.h"

void ScrollBar::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	if ( m_visible ) {
		target.draw( static_cast<sf::RectangleShape>(*this) , states );

		target.draw( topArrow , states );
		target.draw( bottomArrow , states );
		target.draw( upScrollArrow , states );
		target.draw( downScrollArrow , states );

		target.draw( scrollTop , states );
		target.draw( scrollBottom , states );
	}
}

ScrollBar::ScrollBar( float xSize , float ySize ) : Clickable( sf::Vector2f( xSize , ySize ) ) , topArrow() , bottomArrow() , upScrollArrow( 3 ) , downScrollArrow( 3 ) {
	setFillColor( sf::Color( 35 , 35 , 35 ) );

	upScrollArrow.setFillColor( sf::Color( 139 , 139 , 139 ) );
	downScrollArrow.setFillColor( sf::Color( 139 , 139 , 139 ) );

	scrollTop.setFillColor( sf::Color( 100 , 100 , 100 ) );
	scrollTop.setSize( sf::Vector2f( getSize().x - 2 , 1 ) );

	scrollBottom.setFillColor( sf::Color( 100 , 100 , 100 ) );
	scrollBottom.setSize( sf::Vector2f( getSize().x - 2 , 1 ) );

	topArrow.setSize( sf::Vector2f( 13 , 14 ) );
	bottomArrow.setSize( sf::Vector2f( 13 , 14 ) );
}

ScrollBar::~ScrollBar() {

}

void ScrollBar::setPosition( const sf::Vector2f& position ) {
	sf::RectangleShape::setPosition( position );

	upScrollArrow.setPoint( 0 , sf::Vector2f( getPosition().x + 5 , getPosition().y + 9 ) );
	upScrollArrow.setPoint( 1 , sf::Vector2f( getPosition().x + 8 , getPosition().y + 6 ) );
	upScrollArrow.setPoint( 2 , sf::Vector2f( getPosition().x + 11 , getPosition().y + 9 ) );

	downScrollArrow.setPoint( 0 , sf::Vector2f( getPosition().x + 5 , getPosition().y + getSize().y - 9 ) );
	downScrollArrow.setPoint( 1 , sf::Vector2f( getPosition().x + 11 , getPosition().y + getSize().y - 9 ) );
	downScrollArrow.setPoint( 2 , sf::Vector2f( getPosition().x + 8 , getPosition().y + getSize().y - 6 ) );

	scrollTop.setPosition( getPosition().x + 1 , getPosition().y + 15 );
	scrollBottom.setPosition( getPosition().x + 1 , getPosition().y + getSize().y - 17 );

	topArrow.setPosition( sf::Vector2f( getPosition().x + 1 , getPosition().y + 1 ) );
	bottomArrow.setPosition( sf::Vector2f( getPosition().x + 1 , getPosition().y + getSize().y - 15 ) );
}

void ScrollBar::setPosition( float x , float y ) {
	sf::RectangleShape::setPosition( x , y );

	upScrollArrow.setPoint( 0 , sf::Vector2f( getPosition().x + 5 , getPosition().y + 9 ) );
	upScrollArrow.setPoint( 1 , sf::Vector2f( getPosition().x + 8 , getPosition().y + 6 ) );
	upScrollArrow.setPoint( 2 , sf::Vector2f( getPosition().x + 11 , getPosition().y + 9 ) );

	downScrollArrow.setPoint( 0 , sf::Vector2f( getPosition().x + 5 , getPosition().y + getSize().y - 9 ) );
	downScrollArrow.setPoint( 1 , sf::Vector2f( getPosition().x + 11 , getPosition().y + getSize().y - 9 ) );
	downScrollArrow.setPoint( 2 , sf::Vector2f( getPosition().x + 8 , getPosition().y + getSize().y - 6 ) );

	scrollTop.setPosition( getPosition().x + 1 , getPosition().y + 16 );
	scrollBottom.setPosition( getPosition().x + 1 , getPosition().y + getSize().y - 17 );

	topArrow.setPosition( sf::Vector2f( getPosition().x + 1 , getPosition().y + 1 ) );
	bottomArrow.setPosition( sf::Vector2f( getPosition().x + 1 , getPosition().y + getSize().y - 15 ) );
}

void ScrollBar::setSize( const sf::Vector2f& size ) {
	sf::RectangleShape::setSize( size );
	ScrollBar::setPosition( getPosition() ); // resizes all other objects in scroll bar
}

void ScrollBar::setSize( float xSize , float ySize ) {
	sf::RectangleShape::setSize( sf::Vector2f( xSize , ySize ) );
	ScrollBar::setPosition( getPosition() ); // resizes all other objects in scroll bar
}

void ScrollBar::setVisible( bool see ) {
	Clickable::setVisible( see );
	topArrow.setVisible( see );
	bottomArrow.setVisible( see );
}
