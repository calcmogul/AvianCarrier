//=============================================================================
//File Name: MainServer.cpp
//Description: Handles syncing of files on server within AvianCarrier system
//Author: Tyler Veness
//=============================================================================

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include "File.h"

int main() {
	sf::UdpSocket socket;
	socket.bind( 50001 );

	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::Packet packet;

	while ( 1 ) {
		if ( socket.receive( packet , senderIP , senderPort ) == sf::Socket::Done ) {
			File temp( senderIP , senderPort );
			packet >> temp;

			temp.clear();
			packet.clear();
			temp.insert( "The server says hello!" );

			packet << temp;

			socket.send( packet , senderIP , senderPort );
		}
	}

	return 0;
}
