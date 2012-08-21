//=============================================================================
//File Name: ReadWriteProtector.cpp
//Description: Allows resource to be read by many threads but only written to
//             by one thread when no thread is reading
//Author: Tyler Veness
//=============================================================================

#include "ReadWriteProtector.h"
#include <SFML/System/Sleep.hpp>

ReadWriteProtector::ReadWriteProtector() {
	readingMutex = 0;
	writingMutex = 0;
	deletingResource = false;
}

ReadWriteProtector::~ReadWriteProtector() {

}

void ReadWriteProtector::startReading() {
	while ( writingMutex > 0 )
		sf::sleep( sf::milliseconds( 10 ) );

	readingMutex++;
}

void ReadWriteProtector::stopReading() {
	readingMutex--;
}

void ReadWriteProtector::startWriting() {
	while ( readingMutex > 0 && writingMutex > 0 )
		sf::sleep( sf::milliseconds( 10 ) );

	writingMutex++;
}

void ReadWriteProtector::stopWriting() {
	writingMutex--;
}
