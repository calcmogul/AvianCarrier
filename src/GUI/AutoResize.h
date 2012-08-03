//=============================================================================
//File Name: AutoResize.h
//Description: Gives derived class instance the ability to automatically resize
//             when the window is resized
//Author: Tyler Veness
//=============================================================================

#ifndef AUTO_RESIZE_H_
#define AUTO_RESIZE_H_

#include <vector>
#include <SFML/Window/Window.hpp>

class AutoResize {
public:
	static std::vector<AutoResize*> resizeables;

public:
	AutoResize();
	virtual ~AutoResize();

	virtual void updateSize( sf::Window& referTo ) = 0;

	static void resizeAll( sf::Window& referTo );
};

#endif /* AUTO_RESIZE_H_ */
