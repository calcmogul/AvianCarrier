//=============================================================================
//File Name: CheckBox.cpp
//Description: Holds definitions for CheckBox class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include <SFML/Graphics/RenderTarget.hpp>
#include "CheckBox.h"
#include "../Base.h"

CheckBox::CheckBox( float x , float y , bool on , bool see ) : box( sf::Vector2f( 12.f , 12.f ) ) {
	box.setPosition( x , y );
	box.setOutlineThickness( 1.f );

	box.setFillColor( sf::Color( 176 , 180 , 183 ) );
	box.setOutlineColor( sf::Color( 143 , 143 , 143 ) );

	mark.setString( "x" );
	mark.setFont( Base::segoeUI );
	mark.setCharacterSize( 10 );

	mark.setPosition( x + 1 , y + 1 );
	mark.setColor( sf::Color( 0 , 0 , 0 ) );

	m_isChecked = on;
	m_visible = see;
}

bool CheckBox::isChecked() {
	return m_isChecked;
}

bool CheckBox::isVisible() {
	return m_visible;
}

const sf::Vector2f& CheckBox::getPosition() {
	return box.getPosition();
}

void CheckBox::setPosition( float x , float y ) {
	box.setPosition( x , y );
	mark.setPosition( x + 1 , y + 1 );
}

void CheckBox::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	if ( m_visible ) {
		target.draw( box , states );

		if ( m_isChecked )
			target.draw( mark , states );
	}
}
