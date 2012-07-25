//=============================================================================
//File Name: StatusBar.h
//Description: Holds declarations for StatusBar class
//Author: Tyler Veness
//=============================================================================

#ifndef STATUSBAR_H_
#define STATUSBAR_H_

#include <SFML/Graphics/Text.hpp>
#include "AutoResize.h"
#include "TaperRectangleShape.h"

class StatusBar : public sf::Drawable , public AutoResize {
private:
	sf::Text messageBuffer;
	TaperRectangleShape statusBackground;

	void draw( sf::RenderTarget& target , sf::RenderStates states ) const;

public:
	StatusBar();
	virtual ~StatusBar();

	void updateSize( sf::Window& target );
	std::string getStatus();
	void setStatus( std::string message = "" );

	const sf::Vector2f& getSize();
	const sf::Vector2f& getPosition();
};

#endif /* STATUSBAR_H_ */
