//=============================================================================
//File Name: DirList.h
//Description: Declares functions for creating a list of files from a directory
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef DIR_LIST_H_
#define DIR_LIST_H_

#include <string>
#include <SFML/Graphics/Text.hpp>

typedef struct FileList {
	std::string name;
	FileList* next;
} FileList;

FileList* dirList( const char* path );
void freeList( FileList* list );

std::vector<std::string>* getList( std::string path );

#endif /* DIR_LIST_H_ */
