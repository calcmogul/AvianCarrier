//=============================================================================
//File Name: Button.h
//Description: Holds declarations of Button GUI class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef GUI_BUTTON_H_
#define GUI_BUTTON_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
#include "../MenuSelect.h"
#include "Clickable.h"

class Button : public Clickable {
private:
	sf::Color colorVal;
	bool useable;

	std::vector<sf::Keyboard::Key> hotKeyList;
	MenuSelect hotKeyDelay;

	sf::Text sfText;
	std::string text;

	sf::Text sfText2;
	std::string text2;
	short alignment;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	static const unsigned char colorXOR;
	static std::vector<Button*> allButtons;

	static std::map<std::string , sf::Keyboard::Key> hotKeyConverter;
	static bool isLoaded;

	void (*func)();

	// set note to "" for a regular button
	explicit Button( std::string message = "" , std::string note = "" , std::string hotkeys = "" , short textAlign = -1 , unsigned char borderColor = 50 , void (*operation)() = NULL , bool use = true );
	virtual ~Button();

	void setPosition( const sf::Vector2f& position );
	void setPosition( float x , float y );

	void recenterText();
	const std::string& getText();
	const std::string& getText2();

	void setFillColor( sf::Color fill );
	void setFillColor( unsigned char fill );

	sf::Color getFillColor();

	void setUseable( bool use );
	bool isUseable();

	bool hotKeyActivated();
};


#endif /* GUI_BUTTON_H_ */
