//=============================================================================
//File Name: TextReceiver.h
//Description: Holds declarations for TextReceiver class
//Author: Tyler Veness
//=============================================================================

#ifndef TEXT_RECEIVER_H_
#define TEXT_RECEIVER_H_

#include <vector>
#include "Clickable.h"

class TextReceiver : public Clickable {
protected:
	bool m_active;

public:
	static std::vector<TextReceiver*> textInterfaces;
	static TextReceiver* currentReceiver;

	TextReceiver( const sf::Vector2f& size = sf::Vector2f() , std::vector<sf::Color> borders = std::vector<sf::Color>({}) );
	virtual ~TextReceiver();

	static void setReceiver( TextReceiver* activated );
	static void checkSwitchReceiver( sf::Window& referTo );

	virtual void handleEvent( sf::Event& event ) = 0;

	bool isReceiving();

	void activate( bool active ); // changes background of editor : white = active , gray = inactive
	bool isActive();
};

#endif /* TEXT_RECEIVER_H_ */
