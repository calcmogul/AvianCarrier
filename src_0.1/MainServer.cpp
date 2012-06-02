//=============================================================================
//File Name: MainServer.cpp
//Description: Handles syncing of files on server within AvianCarrier system
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include "File.h"

int main() {
	bool running = true;

	sf::UdpSocket socket;
	socket.bind( 50001 );

	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::Packet packet;

	while ( running ) {
		if ( socket.receive( packet , senderIP , senderPort ) == sf::Socket::Done ) {
			File temp( senderIP , senderPort );
			packet >> temp;

			temp.clear();
			packet.clear();
			temp.insert( "Hello!" );

			packet << temp;

			socket.send( packet , senderIP , senderPort );
		}
	}

	return 0;
}
