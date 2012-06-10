//=============================================================================
//File Name: Base.h
//Description: Holds declarations for Base class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef GUI_BASE_H_
#define GUI_BASE_H_

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <SFML/Network/IpAddress.hpp>

#include <SFML/System/Mutex.hpp>

#include "GUI/GUISettings.h"

/* ===== STATIC VAR DECLARATIONS ===== */
extern std::string rootDirectory;
extern std::string searchDir;
extern sf::RenderWindow mainWin;
extern sf::Mutex globalMutex;
/* =================================== */

bool keyPressed( sf::Event& event , sf::Keyboard::Key );
bool keyReleased( sf::Event& event , sf::Keyboard::Key );
bool mouseButtonReleased( sf::Event& event , sf::Mouse::Button button );
bool pressedControl( sf::Event& event );
bool pressedShift( sf::Event& event );
bool pressedAlt( sf::Event& event );
bool mousePressed( sf::Mouse::Button );

typedef struct ServerEntry {
	std::string name;
	sf::IpAddress IP;
	unsigned short port;
} serverEntry;

extern std::vector<ServerEntry> serverList;

class Base {
public:
	static sf::Font consolas;
	static sf::Font segoeUI;

	static std::vector<sf::Texture> fileImages;

	Base();

private:
	static bool isLoaded;
};

#endif /* GUI_BASE_H_ */
