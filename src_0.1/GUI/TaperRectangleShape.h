//=============================================================================
//File Name: TaperRectangleShape.h
//Description: Holds declarations for TaperRectangleShape class
//Author: Tyler Veness
//=============================================================================

#ifndef TAPER_RECTANGLE_SHAPE_H_
#define TAPER_RECTANGLE_SHAPE_H_

#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>

class TaperRectangleShape : public sf::RectangleShape {
private:
	std::vector<sf::RectangleShape*> edges;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	explicit TaperRectangleShape( const sf::Vector2f& size = sf::Vector2f() , std::vector<sf::Color> borders = std::vector<sf::Color>({}) );
	virtual ~TaperRectangleShape();

	void setPosition( const sf::Vector2f& position );
	void setPosition( float x , float y );

	void setSize( const sf::Vector2f& size );
	void setSize( float xSize , float ySize );

	void pushEdge( const sf::Vector2f& size = sf::Vector2f() , const sf::Color& color = sf::Color() );
	void setEdgeColor( unsigned int index , const sf::Color& color );
};

#endif /* TAPER_RECTANGLE_SHAPE_H_ */
