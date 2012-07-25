//=============================================================================
//File Name: MenuSelect.h
//Description: Holds declarations of MenuSelect class
//Author: Tyler Veness
//=============================================================================

#ifndef MENUSELECT_H_
#define MENUSELECT_H_

#include <SFML/System/Clock.hpp>

class MenuSelect {
private:
	unsigned int listSize;

	int lastSelection;
	int selection;

	sf::Clock keyRepeatTime;
	int keyPressWaitTime;

	bool upOldPressed;
	bool upNewPressed;

	bool downOldPressed;
	bool downNewPressed;

public:
	explicit MenuSelect( unsigned int size = 2 );
	virtual ~MenuSelect();

	const int& getSelection();
	const int& getLastSelection();

	void setSelection( int select );

	void setSize( unsigned int size );

	// selection is variable holding current position in menu
	bool updateMove( bool upPressed , bool downPressed ); // @return did selection move ; upPressed should be condition for moving up (e.g. button pressed) , downPressed is opposite
};

#endif /* MENUSELECT_H_ */
