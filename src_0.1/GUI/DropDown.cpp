//=============================================================================
//File Name: DropDown.cpp
//Description: Holds definitions of DropDown GUI class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include <SFML/Graphics/RenderTarget.hpp>
#include "../Base.h"
#include "DropDown.h"
#include "Button.h"

std::vector<DropDown*> DropDown::dropDowns;
DropDown* DropDown::currentOpen = NULL;
DropDown* DropDown::oldOpen = NULL;
bool DropDown::firstUse = true;

DropDown::DropDown( unsigned int x , unsigned int y , std::vector<Button*> menuList ) {
	position.x = x;
	position.y = y;
	menu = menuList;
	open = false;

	dropDowns.push_back( this );
}

DropDown::~DropDown() {
	std::vector<DropDown*>::iterator index;
	for ( index = dropDowns.begin() ; *index != this ; index++ ) {
		if ( index >= dropDowns.end() )
			return;
	}

	dropDowns.erase( index );
}

bool DropDown::headerClicked( sf::Window& referTo ) {
	return mousePressed( sf::Mouse::Left ) && menu[0]->isHovered( referTo );
}

const sf::Vector2f& DropDown::getPosition() {
	return position;
}

void DropDown::setPosition( float x , float y ) {
	position = { x , y };
}

void DropDown::draw( sf::RenderTarget& target , sf::RenderStates states ) const { //menu is out, so draw options and let user choose
	float currentY = position.y;

	for ( unsigned int index = 0 ; index < menu.size() ; index++ ) {
		menu[index]->setPosition( position.x , currentY );
		menu[index]->setVisible( true );

		target.draw( *menu[index] , states );

		currentY += 21.f;
	}
}

void DropDown::closeMenu() {
	open = false;
	firstUse = true;

	for ( unsigned int eraseIdx = 1 ; eraseIdx < menu.size() ; eraseIdx++ )
		menu[eraseIdx]->setVisible( false );

	currentOpen = NULL;
}

void DropDown::switchMenu( DropDown* switchToThis ) {
	open = false;

	for ( unsigned int eraseIdx = 1 ; eraseIdx < menu.size() ; eraseIdx++ )
		menu[eraseIdx]->setVisible( false );

	currentOpen = switchToThis;
	currentOpen->open = true;
}

bool DropDown::isHovered( sf::Window& referTo ) {
	for ( unsigned int index = 0 ; index < menu.size() ; index++ ) {
		if ( menu[index]->isHovered( referTo ) )
			return true;
	}

	return false;
}

void DropDown::checkAllClicked( sf::Window& referTo ) {
	for ( unsigned int index = 0 ; index < dropDowns.size() ; index++ ) {
		if ( dropDowns[index]->headerClicked( referTo ) && dropDowns[index]->menu.size() > 1 ) {
			currentOpen = dropDowns[index];
			currentOpen->open = true;
			firstUse = true;
		}
		else if (dropDowns[index]->menu[0]->func != NULL )
			dropDowns[index]->menu[0]->func();
	}
}
