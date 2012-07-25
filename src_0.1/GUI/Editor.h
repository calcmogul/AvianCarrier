//=============================================================================
//File Name: Editor.h
//Description: Holds declarations for Editor class
//Author: Tyler Veness
//=============================================================================

#ifndef EDITOR_H_
#define EDITOR_H_

#include <SFML/Graphics/RenderTarget.hpp>
#include "../TextReceiver.h"
#include "AutoResize.h"
#include "ScrollBar.h"

class Editor : public TextReceiver , public AutoResize {
private:
	ScrollBar vScroll;

	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	Editor();
	Editor( unsigned int x , unsigned int y , unsigned int width , unsigned int height );
	virtual ~Editor();

	void handleEvent( sf::Event& event );

	void activate( bool active ); // changes background of editor : white = active , gray = inactive

	void setPosition( const sf::Vector2f& position );

	void setSize( const sf::Vector2f& size );

	void updateSize( sf::Window& target );
};

#endif /* EDITOR_H_ */
