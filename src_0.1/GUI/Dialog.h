//=============================================================================
//File Name: Dialog.h
//Description: Holds declarations for Dialog class
//Author: Tyler Veness
//=============================================================================

#ifndef DIALOG_H_
#define DIALOG_H_

#include <string>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../MenuSelect.h"
#include "../TextReceiver.h"

class Dialog {
private:
	MenuSelect keyDelay;

	sf::Text drawInput;
	std::string input;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	Dialog( const sf::Vector2f& size , std::vector<sf::Color> colors );
	virtual ~Dialog();

	void handleEvent( sf::Event& event );

	void setPosition( const sf::Vector2f& position );
};

#endif /* DIALOG_H_ */
