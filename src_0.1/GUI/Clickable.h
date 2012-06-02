//=============================================================================
//File Name: Clickable.h
//Description: Deriving from this class gives derived class clickable nature
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef CLICKABLE_H_
#define CLICKABLE_H_

#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include "TaperRectangleShape.h"

class Clickable : public TaperRectangleShape {
protected:
	bool m_visible;

public:
	Clickable( const sf::Vector2f& size = sf::Vector2f() , std::vector<sf::Color> borders = std::vector<sf::Color>({}) );
	~Clickable();

	bool isHovered( sf::Window& referTo );
	bool isClicked( sf::Window& referTo );

	void setVisible( bool see );
	bool isVisible();
};

#endif /* CLICKABLE_H_ */
