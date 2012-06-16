//=============================================================================
//File Name: CheckBox.h
//Description: Holds declarations for CheckBox class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class CheckBox {
private:
	sf::RectangleShape box;
	sf::Text mark;
	bool m_isChecked;
	bool m_visible;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	CheckBox( float x , float y , bool on = false , bool see = false );

	bool isChecked();
	bool isVisible();

	const sf::Vector2f& getPosition();
	void setPosition( float x , float y );
};

#endif /* CHECKBOX_H_ */
