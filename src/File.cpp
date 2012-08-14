//=============================================================================
//File Name: File.cpp
//Description: Holds definitions for File class
//Author: Tyler Veness
//=============================================================================

#include "File.h"

sf::UdpSocket File::syncSocket;
sf::UdpSocket File::openSocket;
bool File::areSocketsBound = false;

File::File( sf::IpAddress address , std::string path ) : serverIP( address ) {
	fullPath = path;

	if ( path.rfind( "/" ) < path.length() )
		fileName = path.substr( path.rfind( "/" ) + 1 );
	else
		fileName = path;

	if ( fullPath.substr( 0 , 7 ) == "server:" ) {
		if ( bindSockets() == sf::Socket::Done ) {
			sf::Packet retrievePacket;
			retrievePacket << "openFile" << fullPath.substr( 7 );

			openSocket.send( retrievePacket , address , serverOpen );
			std::cout << "sent openFile packet\n";

			retrievePacket.clear();
			unsigned short port;
			if ( openSocket.receive( retrievePacket , address , port ) == sf::Socket::Done ) {
				std::cout << "received openFile packet\n";
				std::string command;
				retrievePacket >> command;
				if ( command == "openFile" )
					retrievePacket >> *this;
			}
		}
	}
	else {
		if ( fileName != "" && fileName != "Untitled" )
			loadFromFile( fullPath );
		else
			input.push_back( "" );
	}

	cursorPos = { 0 , 0 };
	allSelected = false;
	bracketMatch = 0;
	tabSpaceCount = 0;
}

File::~File() {

}

sf::Socket::Status File::bindSockets() {
	if ( !areSocketsBound ) {
#ifdef SERVER_BUILD
		if ( syncSocket.bind( serverSync ) == sf::Socket::Done && openSocket.bind( serverOpen ) == sf::Socket::Done ) {
			areSocketsBound = true;
			return sf::Socket::Done;
		}
		else
			return sf::Socket::NotReady;
#else
		if ( syncSocket.bind( clientSync ) == sf::Socket::Done && openSocket.bind( clientOpen ) == sf::Socket::Done ) {
			areSocketsBound = true;
			return sf::Socket::Done;
		}
		else
			return sf::Socket::NotReady;
#endif
	}

	return sf::Socket::Done;
}

void File::unbindSockets() {
	if ( areSocketsBound ) {
		syncSocket.unbind();
		openSocket.unbind();

		areSocketsBound = false;
	}
}

unsigned int File::size() {
	return input.size();
}

std::string& File::operator[]( unsigned int position ) {
	return input[position];
}

std::string& File::at( unsigned int position ) {
	return input.at( position );
}

std::string File::getCurrentCharacters( unsigned int length , int offset ) {
	return at( cursorPos.y ).substr( cursorPos.x + offset , length );
}

std::string& File::getCurrentLine() {
	return at( cursorPos.y );
}

const std::string File::convertToString() {
	std::string temp;

	for ( unsigned int index = 0 ; index < size() ; index++ ) { // add all lines of file's vector to single string with newline chars in between each
		temp += at( index );
		if ( index < size() - 1 )
			temp += "\n";
	}

	return temp;
}

void File::convertToFile( std::string& fileString ) {
	input = {};

	if ( fileString.find( "\n" ) > fileString.length() )
		fileString += "\n";

	unsigned int index = 0;
	while ( index < fileString.length() ) {
		input.push_back( fileString.substr( index , fileString.find( "\n" , index ) - index ) ); // fills vector with substrings using newlines as delimiter (newline non-inclusive)
		index = fileString.find( "\n" , index ) + 1;
	}

	// make sure cursor isn't put in invalid position within new file
	if ( cursorPos.y >= input.size() )
		cursorPos.y = input.size() - 1;
	if ( cursorPos.x > getCurrentLine().length() )
		cursorPos.x = getCurrentLine().length();
}

std::vector<std::string>::iterator File::insert( unsigned int position , std::string insertMe ) {
	return input.insert( input.begin() + position , insertMe );
}

std::vector<std::string>::iterator File::erase( unsigned int position ) {
	return input.erase( input.begin() + position );
}

void File::clear() {
	input = { "" };
	cursorPos = { 0 , 0 };
}

sf::Packet& operator<<( sf::Packet& fileTransport , const File& sendMe ) { // add file data to packet
	fileTransport << sendMe.fileName;
	fileTransport << sendMe.input.size();

	for ( unsigned int index = 0 ; index < sendMe.input.size() ; index++ )
		fileTransport << sendMe.input[index];

	return fileTransport;
}

sf::Packet& operator>>( sf::Packet& fileTransport , File& receiveMe ) { // move packet data to file
	std::string packetOut;

	fileTransport >> receiveMe.fileName;
	receiveMe.bracketMatch = 0;
	receiveMe.tabSpaceCount = 0;

	unsigned int lineCount;
	fileTransport >> lineCount;

	receiveMe.clear();
	for ( unsigned int index = 0 ; index < lineCount ; index++ ) {
		fileTransport >> packetOut;
		receiveMe.input.insert( receiveMe.input.end() , "" );
		receiveMe.input[index] = packetOut;
	}

	return fileTransport;
}

void File::addTabSpace() {
	for ( unsigned int index = 0 ; index < tabSpaceCount ; index++ )
		insert( "\t" );
}

bool File::save( std::string fileName ) {
	std::string tempPath;
	if ( fullPath.substr( 0 , 7 ) == "server:" )
		tempPath = fullPath.substr( 7 );
	else
		tempPath = fullPath;

	std::ofstream localFile( "Documents/" + fullPath );

	if ( localFile.is_open() ) {
		for ( unsigned int index = 0 ; index < size() ; index++ ) {
			localFile << at(index);

			if ( index < size() - 1 ) // only add newline if there is another line after current one
				localFile << "\n";
		}

		localFile.close();
	}
	else {
		std::cout << "Failed to save file to local directory: Documents/" << tempPath << "\n";
		return false;
	}

	return true;
}

void File::loadFromFile( std::string path ) {
	clear();

	std::fstream file( "Documents/" + path );

	if ( !file.is_open() )
		return;

	std::string data;
	while ( !file.eof() ) {
		std::getline( file , data );

		if ( size() < cursorPos.y + 1 )
			input.insert( input.end() , "" ); // add newline to end of file

		insert( data );
		cursorPos.x = 0;
		cursorPos.y++;
	}
}

unsigned char File::sendToIP() {
	unsigned char status = 0;

	// Compute diff
	inputString = convertToString();
	dtl::Diff<unsigned char , std::string> fileDiff( inputStringShadow , inputString );
	fileDiff.compose();

	// Put file into packet for transmission over network
	fileTransport.clear();
	fileTransport << fileDiff;

	// Set socket for communicating with the server
	syncSocket.setBlocking( false );

	// Send a message to the server
	if ( syncSocket.send( fileTransport , serverIP , serverSync ) != sf::Socket::Done )
		return status;
	else
		status |= File::sent;

	return status;
}

unsigned char File::receiveFromAny() {
	unsigned char status = 0;

	// Receive an answer
	sf::IpAddress recvIP;
	unsigned short recvPort;
	dtl::Diff<unsigned char , std::string> fileDiff;

	syncSocket.setBlocking( false );

	if ( syncSocket.receive( fileTransport , recvIP , recvPort ) != sf::Socket::Done )
		return status;
	else
		status |= File::received;

	fileTransport >> fileDiff;
	inputString = convertToString();
	fileDiff.patch( inputString );
	convertToFile( inputString );

	return status;
}
