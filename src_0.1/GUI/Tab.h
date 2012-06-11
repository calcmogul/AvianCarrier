//=============================================================================
//File Name: Tab.h
//Description: Holds declarations for Tab class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef TAB_H_
#define TAB_H_

#include <string>
#include <vector>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Mutex.hpp>

#include "RenderFile.h"
#include "Button.h"
#include "AutoResize.h"
#include "TaperRectangleShape.h"

class Tab : public Button , public AutoResize {
private:
	static sf::Clock tabCloseWait;

	sf::Text title;
	sf::Text closeX;

	sf::RectangleShape tabBaseLineEraser;

	static short tabPos;
	static bool baseInit;

	Tab( sf::IpAddress address , unsigned short port , std::string fileName );

	void drawTab( sf::RenderTarget& target );

public:
	RenderFile* file;

	static sf::Mutex tabMutex;
	static std::vector<Tab*> tabsOpen;
	static Tab* current;
	static TaperRectangleShape tabBase;
	static unsigned int tabIndex;

	virtual ~Tab();

	std::string getTitle();

	void saveLocal();

	bool isXHovered( sf::Window& referTo );

	void updateSize( sf::Window& referTo );

	static void newTab( sf::IpAddress address , unsigned short port , std::string fileName = "Untitled" ); // uses tabMutex; can block

	static void draw( sf::RenderTarget& target ); // uses tabMutex; can block

	static void checkTabClose( sf::Window& referTo ); // uses tabMutex; can block

	void closeTab(); // uses tabMutex; can block
};

#endif /* TAB_H_ */
