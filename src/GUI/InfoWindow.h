//=============================================================================
//File Name: InfoWindow.h
//Description: Holds declarations for InfoWindow class
//Author: Tyler Veness
//=============================================================================

#ifndef INFO_WINDOW_H
#define INFO_WINDOW_H

#include "../Base.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>

namespace sf {

class InfoWindow {
private:
	RenderWindow sfWindow;
	RenderWindow* parentWindow;
	Event event;

	sf::RenderStates shaderState;
	sf::Shader shader;

public:
	InfoWindow( const char* lpWindowName,
			int nWidth,
			int nHeight,
			sf::RenderWindow* parentWindow,
			sf::Uint32 style,
			sf::Image* icon = NULL,
			std::string shaderFile = "",
			std::string shaderType = "frag" ); // can either be "frag" or "vert" for fragment or vertex shader

	virtual ~InfoWindow();

	void clear( const Color& color );
	void print( std::string message , short x , short y , unsigned int size = 12 , const Color& color = Color( 255 , 255 , 255 ) );
	void setTitle( const std::string& title );
	void display();

	template<class T>
	void setShaderParam( const std::string& name , T var );

	void setPosition( Vector2i position );

	bool checkEvent();
	void waitForExit();
};

#include "setShaderParam.inl"

}

#endif /* INFO_WINDOW_H */
