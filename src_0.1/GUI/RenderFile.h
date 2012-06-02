//=============================================================================
//File Name: RenderFile.h
//Description: Holds declarations for RenderFile derived class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef RENDERFILE_H_
#define RENDERFILE_H_

#include "../File.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class RenderFile : public File , public sf::Drawable {
private:
	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	RenderFile( sf::IpAddress address , unsigned short port , std::string fileName = "" , std::string dir = "" );
	virtual ~RenderFile();

	int lineRenderStart; // number of first line to display in editor window
};

#endif /* RENDERFILE_H_ */
