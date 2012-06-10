//=============================================================================
//File Name: ChatWindow.cpp
//Description: Holds definitions for ChatWindow class
//Author: Tyler Veness
//Last Modified: 5/25/2012
//Version: 0.1
//=============================================================================

#include "Base.h"
#include "GUI/ChatWindow.h"

ChatWindow* ChatWindow::instance = NULL;

ChatWindow::ChatWindow( const sf::Vector2f& size , std::vector<sf::Color> borders ) : TextReceiver( size , borders ) , input( sf::Vector2f( 200.f , 60.f ) , std::vector<sf::Color>({ sf::Color( 45 , 45 , 45 ) , sf::Color( 100 , 100 , 100 ) , sf::Color( 100 , 100 , 100 ) , sf::Color( 45 , 45 , 45 ) }) ) {
	setOutlineThickness( 1.f );
	setFillColor( sf::Color( 255 , 255 , 255 ) );
	setOutlineColor( sf::Color( TAB_FOCUS , TAB_FOCUS , TAB_FOCUS ) );
}

void ChatWindow::createInstance( const sf::Vector2f& size , std::vector<sf::Color> borders ) {
	if ( instance != NULL )
		delete instance;
	instance = new ChatWindow( size , borders );
}

ChatWindow* ChatWindow::getInstance() {
	return instance;
}

void ChatWindow::draw( sf::RenderTarget& target , sf::RenderStates states ) const {
	if ( m_visible ) {
		target.draw( static_cast<RectangleShape>(*this) , states );
		target.draw( input , states );

		sf::Text histText( "" , Base::segoeUI , 12 );
		histText.setColor( sf::Color( 0 , 0 , 0 ) );

		float currentPosition = input.getPosition().y - histText.getCharacterSize();

		for ( int index = history.size() - 1 ; index >= 0 ; index-- ) {
			for ( unsigned int pos = history[index].size() - 1 ; pos >= 0 ; pos-- ) {
				histText.setString( history[index][pos] );

				currentPosition--;
				histText.setPosition( sf::Vector2f( 1.f , currentPosition * histText.getCharacterSize() ) );

				target.draw( histText );
			}
		}
	}
}

void ChatWindow::handleEvent( sf::Event& event ) {
	input.handleEvent( event );
}

void ChatWindow::setSize( const sf::Vector2f& size ) {
	Clickable::setSize( size );
	input.setSize( sf::Vector2f( size.x , input.getSize().y ) );
}

const sf::Vector2f& ChatWindow::getSize() const {
	return input.getSize();
}

void ChatWindow::updateSize( sf::Window& target ) {
	Clickable::setSize( sf::Vector2f( 200.f , target.getSize().y ) );
	Clickable::setPosition( sf::Vector2f( target.getSize().x - Clickable::getSize().x , 0.f ) );
	input.setPosition( sf::Vector2f( getPosition().x , getPosition().y + Clickable::getSize().y - input.getSize().y ) );
}

void ChatWindow::setPosition( const sf::Vector2f& position ) {
	Clickable::setPosition( position );
	input.setPosition( sf::Vector2f( position.x , position.y + Clickable::getSize().y - input.getSize().y ) );
}

void ChatWindow::setVisible( bool visible ) {
	Clickable::setVisible( visible );
	input.setVisible( visible );
}

void ChatWindow::toggleChat() {
	if ( instance->isVisible() ) { // close chat window
		instance->setVisible( false );
		mainWin.setSize( sf::Vector2u( mainWin.getSize().x - instance->getSize().x , mainWin.getSize().y ) );
	}
	else { // open chat window
		instance->setVisible( true );
		mainWin.setSize( sf::Vector2u( mainWin.getSize().x + instance->getSize().x , mainWin.getSize().y ) );
	}
}

void ChatWindow::sendToIP() {

}

void ChatWindow::receiveFromAny() {

}
