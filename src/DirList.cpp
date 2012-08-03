//=============================================================================
//File Name: DirList.cpp
//Description: Defines functions for creating a list of files from a directory
//Author: Tyler Veness
//=============================================================================

#include "DirList.h"
#include <dirent.h>

FileList* dirList( const char* path ) {
	DIR* directory;
	struct dirent* entry = NULL;
	FileList* firstFile = NULL;
	FileList* lastFile = NULL;
	FileList* nextFile = NULL;

	directory = opendir( path );

	do {
		nextFile = new FileList;
		nextFile->next = NULL;

		if( lastFile != NULL )
			lastFile->next = nextFile;
		else
			firstFile = nextFile;

		lastFile = nextFile;
		entry = readdir( directory );

		if ( entry != NULL )
			nextFile->name += entry->d_name;
	} while( entry != NULL );

	return firstFile;
}

void freeList( FileList* list ) {
	if ( list != NULL ) {
		if ( list->next != NULL )
			freeList( list->next ); // frees nested pointers held before freeing self

		delete list->next;
		list->next = NULL;
	}
}

std::vector<std::string>* getList( std::string path ) {
	FileList* fileChainStart = dirList( path.c_str() );
	FileList* fileChain = fileChainStart;

	unsigned char fileCount = 0;

	std::vector<std::string>* fileList = new std::vector<std::string>;

	/* ===== Create list of files in Documents folder ===== */
	if ( fileChain != NULL ) {
		while ( fileChain->next != NULL ) {
			fileCount++;

			if ( fileChain->name != "." && fileChain->name != ".." )
				fileList->push_back( fileChain->name );

			fileChain = fileChain->next;
		}
	}

	fileChain = NULL;
	freeList( fileChainStart );
	/* ==================================================== */

	if ( fileCount > 0 )
		return fileList;
	else
		return NULL;
}
