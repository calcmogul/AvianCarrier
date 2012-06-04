//=============================================================================
//File Name: InputBox.h
//Description: Holds declarations for InputBox class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef INPUTBOX_H_
#define INPUTBOX_H_

#include <string>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../MenuSelect.h"
#include "../TextReceiver.h"

class InputBox : public TextReceiver {
private:
	MenuSelect keyDelay;

	sf::Text drawInput;
	std::string input;

	unsigned int cursorPos;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	InputBox( const sf::Vector2f& size , std::vector<sf::Color> colors );
	virtual ~InputBox();

	void handleEvent( sf::Event& event );

	void setPosition( const sf::Vector2f& position );

	void sendToIP();
	void receiveFromAny();
};

#endif /* INPUTBOX_H_ */
