//=============================================================================
//File Name: File.h
//Description: Holds declarations for File class
//Author: Tyler Veness
//=============================================================================

#ifndef FILE_H_
#define FILE_H_

#include <fstream>
#include <vector>
#include <queue>

#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/System/Vector2.hpp>

#include "dtl/dtl.hpp"

struct Edit {
	dtl::Diff<char , std::string> fileDiff;
	unsigned long long clientVersion;
};

class File {
private:
	static bool areSocketsBound;

protected:
	//std::vector<std::string> input;

public:
	static sf::UdpSocket syncSocket; // handles file syncing with others
	static sf::UdpSocket openSocket; // handles sending and receiving file list between client and server

	enum socketLabel {
		clientSync = 50001,
		clientOpen = 50002,
		serverSync = 50003,
		serverOpen = 50004
	};

	enum netStatus {
		fail,
		sent,
		received
	};

	std::string inputString;
	std::string inputStringShadow;
	unsigned long long clientVersion;
	unsigned long long serverVersion;
	std::queue<Edit> editQueue;

	std::string fullPath;
	std::string fileName;
	std::vector<std::string> input;
	sf::Vector2u cursorPos;
	bool allSelected;
	int bracketMatch;
	unsigned int tabSpaceCount;

	sf::Packet fileTransport;
	sf::IpAddress serverIP;
	unsigned short serverPort;

	explicit File( sf::IpAddress address , unsigned short port , std::string path = "" );
	virtual ~File();

	static sf::Socket::Status bindSockets();

	template <typename T>
	void insert( T newchar );

	unsigned int size();
	std::string& operator[]( unsigned int position );
	std::string& at( unsigned int position );
	std::string getCurrentCharacters( unsigned int length , int offset = 0 );
	std::string& getCurrentLine();

	const std::string convertToString(); // add all lines of file's vector to single string with newline chars in between each ; used in diff
	void convertToFile( std::string& fileString ); // converts string back to file after patch is performed

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
