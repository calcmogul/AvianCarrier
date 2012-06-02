//=============================================================================
//File Name: Toolbar.h
//Description: Holds declarations of Toolbar GUI class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef GUI_TOOLBAR_H_
#define GUI_TOOLBAR_H_

#include <vector>

#include <SFML/Graphics/RectangleShape.hpp>
#include "Clickable.h"
#include "DropDown.h"

class Toolbar : public Clickable {
private:
	float yPos;

	sf::RectangleShape backgroundEdge;
	std::vector<DropDown*> tools;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	Toolbar( float x , float y , float width , float height , std::vector<DropDown*> );

	void setVisible( bool visible );
	void updateSize( sf::Window& target );
};


#endif /* GUI_TOOLBAR_H_ */
