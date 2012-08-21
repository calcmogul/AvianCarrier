//=============================================================================
//File Name: ReadWriteProtector.h
//Description: Allows resource to be read by many threads but only written to
//             by one thread when no thread is reading
//Author: Tyler Veness
//=============================================================================

#ifndef READ_WRITE_PROTECTOR_H
#define READ_WRITE_PROTECTOR_H

#include <SFML/System/Mutex.hpp>

class ReadWriteProtector {
private:
	//sf::Mutex readingMutex;
	//sf::Mutex writingMutex;
	int readingMutex;
	int writingMutex;
	bool deletingResource;

public:
	ReadWriteProtector();
	virtual ~ReadWriteProtector();

	void startReading();
	void stopReading();

	void startWriting();
	void stopWriting();
};

#endif /* READ_WRITE_PROTECTOR_H */
