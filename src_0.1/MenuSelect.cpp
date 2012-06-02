//=============================================================================
//File Name: MenuSelect.cpp
//Description: Holds definitions of MenuSelect class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include "MenuSelect.h"

MenuSelect::MenuSelect( unsigned int size ) {
	listSize = size;

	keyPressWaitTime = 0;

	upOldPressed = false;
	upNewPressed = false;

	downOldPressed = false;
	downNewPressed = false;
}

MenuSelect::~MenuSelect() {

}

const int& MenuSelect::getSelection() {
	return selection;
}

const int& MenuSelect::getLastSelection() {
	return lastSelection;
}

void MenuSelect::setSelection( int select ) {
	selection = select;
}

void MenuSelect::setSize( unsigned int size ) {
	listSize = size;
}

bool MenuSelect::updateMove( bool upPressed , bool downPressed ) {
	lastSelection = selection;

	if ( !(upPressed || downPressed) )
		keyPressWaitTime = 0;

	if ( keyRepeatTime.getElapsedTime().asMilliseconds() >= keyPressWaitTime ) {
		upNewPressed = upPressed;
		downNewPressed = downPressed;

		if ( (!upOldPressed && upNewPressed) || (!downOldPressed && downNewPressed) )
			keyPressWaitTime = 400;
		else if ( (upNewPressed && upOldPressed) || (downNewPressed && downOldPressed) )
			keyPressWaitTime = 100;
		else if ( !(upNewPressed || downNewPressed) )
			keyPressWaitTime = 0;

		if ( upNewPressed || downNewPressed )
			keyRepeatTime.restart();

		if ( upNewPressed ) {
			if ( selection > 0 )
				selection--;
			else
				selection = listSize - 1;
		}

		else if ( downNewPressed ) {
			if ( selection < static_cast<int>(listSize) - 1 )
				selection++;
			else
				selection = 0;
		}

		upOldPressed = upNewPressed;
		downOldPressed = downNewPressed;
	}

	return selection != lastSelection;
}
