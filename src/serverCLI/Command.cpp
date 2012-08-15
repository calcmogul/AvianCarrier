//=============================================================================
//File Name: Command.cpp
//Description: Holds definitions for CommandLine commands
//Author: Tyler Veness
//=============================================================================

#include "Command.h"

bool Command::exitFlag = false;
std::string Command::input = "";
std::string Command::cmd = "";
std::string Command::directory = ".";
unsigned int Command::index = 0;

std::vector<Command*> Command::commands;

Command::Command( std::string cmdName , std::string help , void (*func)() ) {
	name = cmdName;
	execute = func;
	helpMessage = help;
	commands.push_back( this );
}

Command::~Command() {
	std::vector<Command*>::iterator index;
	for ( index = commands.begin() ; *index != this ; index++ ) {
		if ( index == commands.end() )
			return;
	}

	commands.erase( index );
}

std::string Command::getName() {
	return name;
}

const std::string& Command::currentArg() {
	return cmd;
}

void Command::getInput( const char* leadText ) {
	std::cout << leadText;
	std::getline( std::cin , Command::input );
}

void Command::interpret() {
	for ( index = 0 ; index < input.length() ; index++ ) {
		getNextArgument();

		bool recognized = false;
		for ( unsigned int cmdIndex = 0 ; cmdIndex < commands.size() ; cmdIndex++ ) {
			if ( cmd == commands[cmdIndex]->name ) {
				commands[cmdIndex]->execute();
				recognized = true;
			}

			if ( exitFlag == true )
				cmdIndex = commands.size();
		}

		if ( !recognized ) {
			std::cout << cmd << ": command not found\n";
			index--;
		}

		if ( exitFlag == true )
			index = input.length();
	}
}

void Command::getNextArgument() {
	if ( input.substr( index , 1 ) == "\"" ) {
		cmd = input.substr( index + 1 , input.find( "\"" , index + 1 ) - index - 1 );
		index += cmd.length() + 1 + ( input.substr( input.length() - 1 , 1 ) == "\"" );
	}
	else {
		cmd = input.substr( index , input.find( " " , index ) - index );
		index += cmd.length();
	}

	while ( input.substr( index , 1 ) == " " && index < input.length() )
		index++;
}

void Command::error( std::string command , std::string badArgument , std::string message ) {
	if ( badArgument != "" )
		std::cout << command << ": " << badArgument << ": " << message << '\n';
	else
		std::cout << command << ": " << message << '\n';
}
