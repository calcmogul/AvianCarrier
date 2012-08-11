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
#include <iostream> //FIXME

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
#if 0
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
#endif

	sf::IpAddress senderIP;
	unsigned short senderPort;
	sf::Packet packet;

	File::bindSockets();
	File::syncSocket.setBlocking( false );
	File::openSocket.setBlocking( false );

	while ( 1 ) {
		if ( File::syncSocket.receive( packet , senderIP , senderPort ) == sf::Socket::Done ) {
			File temp( senderIP , senderPort );
			packet >> temp;

			temp.clear();
			packet.clear();
			temp.insert( "The server says hello!" );

			packet << temp;

			File::syncSocket.send( packet , senderIP , senderPort );
		}

		if ( File::openSocket.receive( packet , senderIP , senderPort ) == sf::Socket::Done ) {
			std::string command;
			packet >> command;

			if ( command == "dirList" ) {
				std::string searchDirectory;
				packet >> searchDirectory;
				std::cout << "dirList: " << searchDirectory << "\n";

				std::vector<std::string>* tempList = getList( searchDirectory );

				packet.clear();
				packet << "dirList";
				if ( tempList != NULL ) { // may be NULL if the user sent a filename to list instead of a directory
					for ( unsigned int index = 0 ; index < tempList->size() ; index++ ) {
						packet << (*tempList)[index];
					}
				}

				File::openSocket.send( packet , senderIP , senderPort );
			}

			if ( command == "openFile" ) {
				std::string fileName;
				packet >> fileName;
				std::cout << "openFile: " << fileName << "\n";

				File fileToSend( senderIP , senderPort );
				fileToSend.loadFromFile( fileName );

				packet.clear();
				packet << fileToSend;
				File::openSocket.send( packet , senderIP , senderPort );
			}
		}
	}

	return 0;
}
