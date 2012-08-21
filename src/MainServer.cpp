//=============================================================================
//File Name: MainServer.cpp
//Description: Handles syncing of files on server within AvianCarrier system
//Author: Tyler Veness
//=============================================================================

#if 0
#include <SFML/Network/TcpListener.hpp>
#include <list>
#endif

#include <SFML/Network/TcpSocket.hpp>

#include <SFML/Network/SocketSelector.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Thread.hpp>
#include "serverCLI/CommandFunctions.h"
#include "File.h"
#include "DirList.h"
#include <fstream>

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

bool CLOSE_THREADS = false;

void serverCLI() {
	Command cat( "cat" , "concatenate files and print on the standard output" , []{ CmdFunc::cat(); } );
	Command cd( "cd" , "change the working directory" , []{ CmdFunc::cd(); } );
	Command exit( "exit" , "exit program" , [] { CmdFunc::exit(); } );
	Command help( "help" , "list help info for all commands - takes: [opt] command" , [] { CmdFunc::help(); } );
	Command ls( "ls" , "list files with current filetype - takes: [opt] File name" , [] { CmdFunc::ls(); } );
	Command mv( "mv" , "rename file - takes: old file name , new file name" , []{ CmdFunc::mv(); } );
	Command pwd( "pwd" , "print name of current/working directory" , []{ CmdFunc::pwd(); } );
	Command rm( "rm" , "remove files or directories - takes: File name" , [] { CmdFunc::rm(); } );

	while ( !CLOSE_THREADS ) {
		Command::getInput( "\nAvianServer>" );
		Command::interpret();

		if ( Command::exitFlag == true )
			CLOSE_THREADS = true;
	}
}

int main() {
#if 0
	// Create a socket to listen to new connections
	sf::TcpListener listener;
	listener.listen( File::serverSync );

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

	sf::SocketSelector commandSockets;
	commandSockets.add( File::syncSocket );
	commandSockets.add( File::openSocket );

	freopen( "error.log" , "w+" , stderr );
	std::ofstream log( "serverActivity.log" , std::ofstream::out | std::ofstream::ate );

	sf::Thread commandLine( serverCLI );
	commandLine.launch();

	while ( !CLOSE_THREADS ) {
		if ( commandSockets.wait( sf::milliseconds( 500 ) ) ) {
			if ( commandSockets.isReady( File::syncSocket ) ) {
				File::syncSocket.receive( packet , senderIP , senderPort );
				File temp( senderIP );
				packet >> temp;

				temp.clear();
				packet.clear();
				temp.insert( "The server says hello!" );

				packet << temp;
				packet.clear();

				File::syncSocket.send( packet , senderIP , senderPort );
			}

			if ( commandSockets.isReady( File::openSocket ) ) {
				File::openSocket.receive( packet , senderIP , senderPort );
				std::string command;
				packet >> command;

				if ( command == "dirList" ) {
					std::string searchDirectory;
					packet >> searchDirectory;
					log << "dirList  @ " << senderIP << " : " << searchDirectory << "\n";
					log.flush();

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
					log << "openFile @ " << senderIP << " : " << fileName << "\n";
					log.flush();

					File fileToSend( senderIP );
					fileToSend.loadFromFile( fileName );

					packet.clear();
					packet << "openFile" << fileToSend << "\n";
					File::openSocket.send( packet , senderIP , senderPort );
				}
			}
		}
	}

	log.close();
	File::unbindSockets();

	return 0;
}
