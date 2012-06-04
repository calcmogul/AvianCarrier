//=============================================================================
//File Name: AutoResize.h
//Description: Gives class instance ability to automatically resize when the
//             window is resized
//Author: Tyler Veness
//Last Modified: 6/3/2012
//Version: 0.1
//=============================================================================

#ifndef AUTO_RESIZE_H_
#define AUTO_RESIZE_H_

#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class AutoResize {
public:
	static std::vector<AutoResize*> resizeables;

public:
	AutoResize();
	virtual ~AutoResize();

	virtual void updateSize( sf::Window& referTo ) = 0;

	static void resizeAll( sf::Window& referTo );
	//static void drawAll( sf::RenderWindow& referTo , sf::RenderStates states = sf::RenderStates::Default );
};

#endif /* AUTO_RESIZE_H_ */
