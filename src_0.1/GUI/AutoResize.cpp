//=============================================================================
//File Name: AutoResize.cpp
//Description: Gives derived class instance the ability to automatically resize
//             when the window is resized
//Author: Tyler Veness
//=============================================================================

#include "AutoResize.h"

std::vector<AutoResize*> AutoResize::resizeables;

AutoResize::AutoResize() {
	resizeables.push_back( this );
}

AutoResize::~AutoResize() {
	std::vector<AutoResize*>::iterator index;
	for ( index = resizeables.begin() ; *index != this ; index++ ) {
		if ( index >= resizeables.end() )
			return;
	}

	resizeables.erase( index );
}

void AutoResize::resizeAll( sf::Window& referTo ) {
	for ( unsigned int index = 0 ; index < resizeables.size() ; index++ )
		resizeables[index]->updateSize( referTo );
}
