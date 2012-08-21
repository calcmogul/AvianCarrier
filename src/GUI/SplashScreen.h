//=============================================================================
//File Name: InfoWindow.h
//Description: Holds declarations for SplashScreen class
//Author: Tyler Veness
//=============================================================================

#ifndef SPLASHSCREEN_H_
#define SPLASHSCREEN_H_

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace sf {

class SplashScreen : public RenderWindow {
private:
	sf::Sprite backDraw;
	sf::Texture backTexture;

public:
	SplashScreen( const std::string& imageName = "" );
	SplashScreen( unsigned int modeX , unsigned int modeY , const std::string& title , Uint32 style = Style::Default , const std::string& imageName = "" );
	virtual ~SplashScreen();

	void create( unsigned int modeX , unsigned int modeY , const std::string& title , Uint32 style = Style::Default , const std::string& imageName = "" );

	void drawAll();
	void checkForExitClick();
};

}

#endif /* SPLASHSCREEN_H_ */
