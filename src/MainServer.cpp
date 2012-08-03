//=============================================================================
//File Name: MainServer.cpp
//Description: Handles syncing of files on server within AvianCarrier system
//Author: Tyler Veness
//=============================================================================

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/SocketSelector.hpp>
#include <list>

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include "File.h"
#include "DirList.h"

class Client {
private:
	unsigned long long m_clientVersion;
	unsigned long long m_serverVersion;

public:
	sf::TcpSocket* socket;

	explicit Client( sf::TcpSocket* clientSocket , unsigned long long clientVersion = 0 , unsigned long long serverVersion = 0 ) {
		socket = clientSocket;
		m_clientVersion = clientVersion;
		m_serverVersion = serverVersion;
	}

	virtual ~Client() {
		delete socket;
	}
};

int main() {
	// Create a socket to listen to new connections
	sf::TcpListener listener;
	listener.listen( 50001 );

	// Create a list to store the future clients
	std::list<Client*> clients;

	// Create a selector
	sf::SocketSelector selector;

	// Add the listener to the selector
	selector.add( listener );

	while ( 1 ) {
		// Make the selector wait for data on any socket
		if ( selector.wait() ) {
			// Test the listener
			if ( selector.isReady( listener ) ) {
				// The listener is ready: there is a pending connection
				sf::TcpSocket* clientSocket = new sf::TcpSocket;
				if ( listener.accept( *clientSocket ) == sf::Socket::Done ) {
					// Add the new client to the clients list
					clients.push_back( new Client( clientSocket ) );

					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add( *clientSocket );
				}
			}
			else {
				// The listener socket is not ready, test all other sockets (the clients)
				for ( std::list<Client*>::iterator it = clients.begin() ; it != clients.end() ; ++it ) {
					Client& client = **it;
					if ( selector.isReady( *client.socket ) ) {
						// The client has sent some data, we can receive it
						sf::Packet packet;
						if ( client.socket->receive( packet ) == sf::Socket::Done ) {

						}
					}
				}
			}
		}
	}

	// ====================================================

	sf::UdpSocket syncSocket; // handles file syncing with clients
	syncSocket.bind( 50001 );

	sf::UdpSocket openSocket; // handles sending file list to clients
	openSocket.bind( 50002 );

	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::Packet packet;

	while ( 1 ) {
		if ( syncSocket.receive( packet , senderIP , senderPort ) == sf::Socket::Done ) {
			File temp( senderIP , senderPort );
			packet >> temp;

			temp.clear();
			packet.clear();
			temp.insert( "The server says hello!" );

			packet << temp;

			syncSocket.send( packet , senderIP , senderPort );
		}

		if ( openSocket.receive( packet , senderIP , senderPort ) == sf::Socket::Done ) {

		}
	}

	return 0;
}
