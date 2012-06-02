//=============================================================================
//File Name: Base.h
//Description: Holds declarations for Base class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef GUI_BASE_H_
#define GUI_BASE_H_

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include <SFML/Network/IpAddress.hpp>

#include "GUI/GUISettings.h"

/* ===== STATIC VAR DECLARATIONS ===== */
extern std::string rootDirectory;
extern std::string searchDir;
extern HINSTANCE globalInstance;
/* =================================== */

bool keyPressed( sf::Keyboard::Key );
bool keyReleased( sf::Event& event , sf::Keyboard::Key );
bool mouseButtonReleased( sf::Event& event , sf::Mouse::Button button );
bool pressedControl();
bool pressedShift();
bool pressedAlt();
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
