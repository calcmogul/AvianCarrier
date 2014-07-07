//=============================================================================
//File Name: CommandFunctions.cpp
//Description: Holds definitions for functions of commands
//Author: Tyler Veness
//=============================================================================

#include "CommandFunctions.h"
#include <fstream>
#include "../DirList.h"

namespace CmdFunc {
void cat() {
	Command::getNextArgument();

	std::string tempCatStore;

	// concatenate all other files in command line arguments to temporary file
	while ( Command::currentArg() != "" ) {
		std::ifstream fileToAppend( Command::currentArg().c_str() );

		if ( fileToAppend.good() ) {
			std::string text;

			while ( !fileToAppend.eof() ) {
				std::getline( fileToAppend , text );
				tempCatStore += text;
			}

			fileToAppend.close();
		}
		else {
			std::cout << tempCatStore;
			tempCatStore = "";

			Command::error( "cat" , Command::currentArg() , "No such file or directory" );
		}

		Command::getNextArgument();
	}

	std::cout << tempCatStore;
}

void cd() {
	Command::getNextArgument();

	std::string nextDir = Command::currentArg();

	if ( nextDir == "" )
		std::cout << Command::directory << "\n";
	else {
		std::vector<std::string>* fileList = getList( Command::directory + "/" + nextDir );
		if ( fileList != NULL ) { // if directory exists
			// condense directory name to smallest correct directory listing
			while ( nextDir.length() > 0 ) {
				if ( nextDir.substr( 0 , 2 ) == ".." ) { // if moving to parent directory
					Command::directory = Command::directory.substr( 0 , Command::directory.rfind( "/" ) );
					nextDir = nextDir.substr( 2 );

					if ( nextDir.substr( 0 , 1 ) == "/" )
						nextDir = nextDir.substr( 1 );
				}
				else {
					if ( Command::directory.substr( Command::directory.length() - 1 ) != "/" ) // add "/" to directory end if not already there for adding more from currentArg()
						Command::directory += "/";

					Command::directory += nextDir.substr( 0 , nextDir.find( "/" ) );

					if ( nextDir.find( "/" ) > nextDir.length() )
						nextDir = "";
					else
						nextDir = nextDir.substr( nextDir.find( "/" ) + 1 );


					if ( nextDir.substr( 0 , 1 ) == "/" )
						nextDir = nextDir.substr( 1 );
				}
			}

			if ( Command::directory == "" )
				Command::directory == ".";

			delete fileList;
		}
		else
			Command::error( "cd" , nextDir , "No such file or directory" );
	}
}

void exit() {
	Command::exitFlag = true;
}

void help() {
	bool helped( false );

	Command::getNextArgument();

	for ( unsigned int i = 0 ; i < Command::commands.size() ; i++ ) {
		if ( Command::currentArg() == Command::commands[i]->getName().substr( 0 , Command::currentArg().length() ) || Command::currentArg() == "" ) {
			std::cout << Command::commands[i]->getName();

			for ( unsigned int index = Command::commands[i]->getName().length() ; index < 14 ; index++ )
				std::cout << " ";

			std::cout << Command::commands[i]->helpMessage << "\n";

			helped = true;
		}
	}

	if ( !helped )
		Command::error( "help" , "" , "No help topics match '" + Command::currentArg() + "'" );
}

void ls() {
	Command::getNextArgument();

	std::vector<std::string>* fileList = getList( Command::directory );

	if ( fileList != NULL ) {
		for ( unsigned int index = 0 ; index < fileList->size() ; index++ ) {
			if ( (*fileList)[index] == Command::currentArg() || Command::currentArg() == "" )
				std::cout << (*fileList)[index] << "\n";
		}
	}
	else
		Command::error( "ls" , Command::currentArg() , "No such file or directory" );

	// Destruct fileList
	while ( fileList->size() > 0 ) {
		fileList->erase( fileList->begin() );
	}
	delete fileList;
}

void mv() {
	Command::getNextArgument();
	if ( Command::currentArg().size() > 0 ) {
		std::string oldName = Command::directory + "/" + Command::currentArg();
		Command::getNextArgument();

		if ( Command::currentArg().size() > 0 ) {
			rename( oldName.c_str() , (Command::directory + "/" + Command::currentArg()).c_str() );
		}
		else
			Command::error( "mv" , "cannot stat '" + oldName + "'" , "No such file or directory" );
	}
	else
		Command::error( "mv" , "" , "missing file operand" );
}

void pwd() {
	std::cout << Command::directory << "\n";
}

void rm() {
	Command::getNextArgument();

	if ( Command::currentArg() == "" )
		Command::error( "rm" , "" , "missing operand" );

	while ( Command::currentArg() != "" ) {
		if ( remove( (Command::directory + "/" + Command::currentArg()).c_str() ) != 0 )
			Command::error( "rm" , Command::currentArg() , "No such file or directory" );

		Command::getNextArgument();
	}
}
}
