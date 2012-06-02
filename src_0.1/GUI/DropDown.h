//=============================================================================
//File Name: DropDown.h
//Description: Holds declarations of DropDown GUI class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef GUI_DROPDOWN_H_
#define GUI_DROPDOWN_H_

#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include "../Base.h"
#include <SFML/Window.hpp>
#include "Button.h"

class DropDown : public sf::Drawable {
private:
	sf::Vector2f position;

	bool headerClicked( sf::Window& referTo );

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	static std::vector<DropDown*> dropDowns;
	static DropDown* currentOpen;
	static DropDown* oldOpen;
	static bool firstUse;

	std::vector<Button*> menu;
	bool open;

	DropDown( unsigned int x , unsigned int y , std::vector<Button*> );
	~DropDown();

	const sf::Vector2f& getPosition();
	void setPosition( float x , float y );

	void closeMenu();
	void switchMenu( DropDown* switchToThis );

	bool isHovered( sf::Window& referTo );
	static void checkAllClicked( sf::Window& referTo );
};

#endif /* GUI_DROPDOWN_H_ */
