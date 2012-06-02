//=============================================================================
//File Name: TaperRectangleShape.cpp
//Description: Holds definitions for TaperRectangleShape class
//Author: Tyler Veness
//Last Modified: 5/28/2012
//Version: 0.1
//=============================================================================

#include <SFML/Graphics/RenderTarget.hpp>
#include "GUI/TaperRectangleShape.h"

void TaperRectangleShape::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	for ( unsigned int index = 0 ; index < edges.size() ; index++ )
		target.draw( *edges[index] , states );

	target.draw( static_cast<sf::RectangleShape>(*this) , states );
}

TaperRectangleShape::TaperRectangleShape( const sf::Vector2f& size , std::vector<sf::Color> borders ) : sf::RectangleShape( size ) {
	for ( unsigned int index = 0 ; index < borders.size() ; index++ ) {
		edges.push_back( new sf::RectangleShape( size ) );
		edges[index]->setOutlineThickness( index + 2 );
		edges[index]->setOutlineColor( borders[index] );
	}
}

TaperRectangleShape::~TaperRectangleShape() {
	for ( unsigned int index = 0 ; index < edges.size() ; index++ )
		delete edges[index];
}

void TaperRectangleShape::setPosition( const sf::Vector2f& position ) {
	sf::RectangleShape::setPosition( position );

	for ( unsigned int index = 0 ; index < edges.size() ; index++ )
		edges[index]->setPosition( position );
}

void TaperRectangleShape::setPosition( float x , float y ) {
	sf::RectangleShape::setPosition( x , y );

	for ( unsigned int index = 0 ; index < edges.size() ; index++ )
		edges[index]->setPosition( x , y );
}

void TaperRectangleShape::setSize( const sf::Vector2f& size ) {
	sf::RectangleShape::setSize( size );

	for ( unsigned int index = 0 ; index < edges.size() ; index++ )
		edges[index]->setSize( size );
}

void TaperRectangleShape::setSize( float xSize , float ySize ) {
	sf::RectangleShape::setSize( sf::Vector2f( xSize , ySize ) );

	for ( unsigned int index = 0 ; index < edges.size() ; index++ )
		edges[index]->setSize( sf::Vector2f( xSize , ySize ) );
}

void TaperRectangleShape::pushEdge( const sf::Vector2f& size , const sf::Color& color ) {
	edges.push_back( new sf::RectangleShape( size ) );
	edges[edges.size() - 1]->setOutlineThickness( edges.size() - 1 + 2 );
	edges[edges.size() - 1]->setOutlineColor( color );
}

void TaperRectangleShape::setEdgeColor( unsigned int index , const sf::Color& color = sf::Color() ) {
	edges.at(index)->setFillColor( color );
}
