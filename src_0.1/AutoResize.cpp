//=============================================================================
//File Name: AutoResize.cpp
//Description: Gives class instance ability to automatically resize when the
//             window is resized
//Author: Tyler Veness
//Last Modified: 6/3/2012
//Version: 0.1
//=============================================================================

#include "GUI/AutoResize.h"

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
