//=============================================================================
//File Name: RenderFile.h
//Description: Holds declarations for RenderFile derived class
//Author: Tyler Veness
//=============================================================================

#ifndef RENDERFILE_H_
#define RENDERFILE_H_

#include "../File.h"
#include <SFML/Graphics/RenderTarget.hpp>

class RenderFile : public File , public sf::Drawable {
private:
	void draw( sf::RenderTarget& target , sf::RenderStates states = sf::RenderStates::Default ) const;

public:
	RenderFile( sf::IpAddress address , unsigned short port , std::string path = "" );
	virtual ~RenderFile();

	int lineRenderStart; // number of first line to display in editor window
};

#endif /* RENDERFILE_H_ */
