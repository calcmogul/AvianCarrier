//=============================================================================
//File Name: ChatWindow.h
//Description: Holds declarations for ChatWindow class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef CHATWINDOW_H_
#define CHATWINDOW_H_

#include <vector>
#include <string>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Window.hpp>
#include "InputBox.h"
#include "../Base.h"
#include "../TextReceiver.h"
#include "TaperRectangleShape.h"

class ChatWindow : public TextReceiver {
private:
	std::vector<std::vector<sf::String>> history;
	InputBox input;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	ChatWindow( const sf::Vector2f& size = sf::Vector2f() , std::vector<sf::Color> borders = std::vector<sf::Color>({}) );

	void handleEvent( sf::Event& event );

	void setSize( const sf::Vector2f& size );
	const sf::Vector2f& getSize() const;
	void updateSize( sf::Window& target );

	void setPosition( const sf::Vector2f& position );

	void setVisible( bool visible );

	void sendToIP();
	void receiveFromAny();
};

#endif /* CHATWINDOW_H_ */
