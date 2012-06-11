//=============================================================================
//File Name: File.h
//Description: Holds declarations for File class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef FILE_H_
#define FILE_H_

#include <fstream>
#include <vector>

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Vector2.hpp>

class File {
protected:
	//std::vector<std::string> input;

public:
	enum netStatus {
		fail,
		sent,
		received
	};

	std::string fullPath;
	std::string fileName;
	std::vector<std::string> input;
	sf::Vector2u cursorPos;
	bool allSelected;
	int bracketMatch;
	unsigned int tabSpaceCount;

	sf::UdpSocket serverSocket;
	sf::Packet fileTransport;
	sf::IpAddress serverIP;
	unsigned short serverPort;

	explicit File( sf::IpAddress address , unsigned short port , std::string path = "" );
	virtual ~File();

	template <typename T>
	void insert( T newchar );

	unsigned int size();
	std::string& operator[]( unsigned int position );
	std::string& at( unsigned int position );
	std::string getCurrentCharacters( unsigned int length , int offset = 0 );
	std::string& getCurrentLine();

	std::vector<std::string>::iterator insert( unsigned int position , std::string insertMe );
	std::vector<std::string>::iterator erase( unsigned int position );
	void clear(); // empties file and sets cursor to beginning of file, { 0 , 0 }

	friend sf::Packet& operator<<( sf::Packet& fileTransport , const File& sendMe );
	friend sf::Packet& operator>>( sf::Packet& fileTransport , File& receiveMe );

	void addTabSpace();

	bool save( std::string fileName );
	void loadFromFile( std::string path );

	unsigned char sendToIP();
	unsigned char receiveFromAny();
};

#include "insert.inl"

#endif /* FILE_H_ */
