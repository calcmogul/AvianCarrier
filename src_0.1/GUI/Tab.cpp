 //=============================================================================
//File Name: Tab.cpp
//Description: Holds definitions for Tab class
//Author: Tyler Veness
//=============================================================================

#include "../Base.h"
#include "Tab.h"

sf::Mutex Tab::tabMutex;
sf::Clock Tab::tabCloseWait;

std::vector<Tab*> Tab::tabsOpen;
Tab* Tab::current = NULL;

short Tab::tabPos;
TaperRectangleShape Tab::tabBase( sf::Vector2f() , std::vector<sf::Color>({ sf::Color( 45.f , 45.f , 45.f ) }) );
bool Tab::baseInit = false;
unsigned int Tab::tabIndex = 0;

Tab::Tab( sf::IpAddress address , unsigned short port , std::string fileName ) : Button( fileName.substr( fileName.rfind( "/" ) + 1 ) , "" , "" , BUTTON_SPACING / 2 , 15 , []{} ) , title( fileName , Base::segoeUI , 12 ) , closeX( "x" , Base::segoeUI , 13 ) {
	file = new RenderFile( address , port , fileName );
	closeX.setColor( sf::Color( 30 , 30 , 30 ) );

	setSize( sf::Vector2f( getSize().x + BUTTON_SPACING / 2 + 7 , getSize().y - 1 ) );
	setPosition( 12 + tabPos , TAB_START_Y );
	setFillColor( sf::Color( BUTTON_OFF , BUTTON_OFF , BUTTON_OFF ) );

	tabBaseLineEraser.setSize( sf::Vector2f( getSize().x , 1.f ) );
	tabBaseLineEraser.setFillColor( sf::Color( TAB_FOCUS , TAB_FOCUS , TAB_FOCUS ) );
}

Tab::~Tab() {
	std::vector<Tab*>::iterator index;
	for ( index = tabsOpen.begin() ; *index != this ; index++ ) {
		if ( index >= tabsOpen.end() )
			return;
	}

	delete file;
	tabsOpen.erase( index );
}

std::string Tab::getTitle() {
	return title.getString();
}

void Tab::saveLocal() {
	file->save( "Documents/" + file->fullPath );
}

bool Tab::isXHovered( sf::Window& referTo ) {
	return sf::Mouse::getPosition( referTo ).x >= closeX.getPosition().x && sf::Mouse::getPosition( referTo ).x <= closeX.findCharacterPos( 1 ).x && sf::Mouse::getPosition( referTo ).y >= closeX.getPosition().y + 6 && sf::Mouse::getPosition( referTo ).y <= closeX.getPosition().y + closeX.getCharacterSize() - 1;
}

void Tab::updateSize( sf::Window& referTo ) {
	tabBase.setSize( sf::Vector2f( referTo.getSize().x , Tab::tabBase.getSize().y ) );
}

void Tab::newTab( sf::IpAddress address , unsigned short port , std::string fileName ) {
	tabMutex.lock();

	tabsOpen.push_back( new Tab( address , port , fileName ) );
	current = tabsOpen.at( tabsOpen.size() - 1 );
	current->setVisible( true );

	tabMutex.unlock();
}

void Tab::drawTab( sf::RenderTarget& target ) {
	setPosition( tabPos , getPosition().y );
	tabBaseLineEraser.setPosition( sf::Vector2f( getPosition().x , tabBase.getPosition().y - 1 ) );

	if ( current == this )
		setFillColor( TAB_FOCUS );
	else
		setFillColor( BUTTON_OFF );

	target.draw( static_cast<Button>(*this) );

	if ( current == this )
		target.draw( tabBaseLineEraser );

	closeX.setPosition( tabPos + getSize().x - 15 , getPosition().y + ( getSize().y - 14.f ) / 2 - 1.5 );
	target.draw( closeX );

	tabPos += getSize().x + 4;
}

void Tab::draw( sf::RenderTarget& target ) {
	tabMutex.lock();

	tabPos = TAB_START_X;

	if ( !baseInit ) {
		tabBase.setSize( sf::Vector2f( target.getSize().x , 4.f ) );
		tabBase.setPosition( 0 , TAB_BASE_Y - 1 );
		tabBase.setOutlineThickness( 1.f );
		tabBase.setFillColor( sf::Color( TAB_FOCUS , TAB_FOCUS , TAB_FOCUS ) );
		tabBase.setOutlineColor( sf::Color( 30 , 30 , 30 ) );

		baseInit = true;
	}

	target.draw( tabBase );

	for ( unsigned int index = 0 ; index < tabsOpen.size() ; index++ )
		tabsOpen.at(index)->drawTab( target );

	tabMutex.unlock();
}

void Tab::checkTabClose( sf::Window& referTo ) {
	tabMutex.lock();

	// Close any tabs that had their X clicked
	for ( unsigned int index = 0 ; index < Tab::tabsOpen.size() ; index++ ) {
		if ( tabCloseWait.getElapsedTime().asMilliseconds() > 200 && sf::Mouse::isButtonPressed( sf::Mouse::Left ) && Tab::tabsOpen[index]->isXHovered( referTo ) ) {
			Tab::tabsOpen[index]->closeTab();

			tabCloseWait.restart();
			break;
		}
	}

	tabMutex.unlock();
}

void Tab::closeTab() {
	tabMutex.lock();

	for ( unsigned int index = 0 ; index < tabsOpen.size() ; index++ ) {
		if ( this == tabsOpen.at(index) ) { // have to find current tab first
			if ( this == current ) { // change current tab if it is closing
				if ( index < tabsOpen.size() - 1 ) { // if there is tab to right
					current = tabsOpen.at(index + 1); // make current tab right one
					tabIndex = index;
				}
				else if ( tabsOpen.size() > 1 ) { // else if there is tab to left
					current = tabsOpen.at(index - 1); // make current tab left one
					tabIndex = index - 1;
				}
				else
					current = NULL; //else there are no more tabs left
			}

			tabsOpen.at( index )->~Tab();

			break;
		}
	}

	tabMutex.unlock();
}
