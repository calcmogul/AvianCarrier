//=============================================================================
//File Name: Command.h
//Description: Holds declarations for CommandLine commands
//Author: Tyler Veness
//=============================================================================

#ifndef COMMAND_H_
#define COMMAND_H_

#include <iostream>
#include <string>
#include <vector>

class Command {
private:
	std::string name;
	void (*execute)();

	static std::string cmd;
	static std::string input;
	static unsigned int index;

public:
	static bool exitFlag;
	static std::string directory;

	static std::vector<Command*> commands;

	std::string helpMessage;

	Command( std::string cmdName , std::string help , void (*func)() = NULL );
	~Command();

	std::string getName();

	static const std::string& currentArg();
	static void getInput( const char* leadText );
	static void interpret();
	static void getNextArgument();
	static void error( std::string command , std::string badArgument , std::string message );
};

#endif /* COMMAND_H_ */
