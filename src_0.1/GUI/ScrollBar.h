//=============================================================================
//File Name: ScrollBar.h
//Description: Holds declarations for ScrollBar class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef SCROLLBAR_H_
#define SCROLLBAR_H_

#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Button.h"
#include "Clickable.h"

class ScrollBar : public Clickable {
private:
	sf::ConvexShape upScrollArrow;
	sf::ConvexShape downScrollArrow;
	sf::RectangleShape scrollTop;
	sf::RectangleShape scrollBottom;

	sf::RectangleShape bar;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	Button topArrow;
	Button bottomArrow;

	ScrollBar( float xSize , float ySize );
	virtual ~ScrollBar();

	void setPosition( const sf::Vector2f& position );
	void setPosition( float x , float y );

	void setSize( const sf::Vector2f& size );
	void setSize( float xSize , float ySize );

	void setVisible( bool see );
};

#endif /* SCROLLBAR_H_ */
