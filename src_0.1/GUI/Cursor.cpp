#include "Cursor.hpp"

#ifdef SFML_SYSTEM_WINDOWS
sf::Cursor::Cursor( const sf::Cursor::Type t ) {
    switch(t) {
		case sf::Cursor::Wait:
			cursor = LoadCursor( NULL , IDC_WAIT );
			break;
		case sf::Cursor::Hand:
			cursor = LoadCursor( NULL , IDC_HAND );
			break;
		case sf::Cursor::Normal:
			cursor = LoadCursor( NULL , IDC_ARROW );
			break;
		case sf::Cursor::Text:
			cursor = LoadCursor( NULL , IDC_IBEAM );
			break;
			//For more cursor options on Windows go here: http://msdn.microsoft.com/en-us/library/ms648391%28v=vs.85%29.aspx
	}
}

void sf::Cursor::set( const sf::Window& Window ) const {
	SetClassLong( static_cast<HWND>(Window.getSystemHandle()) , GCL_HCURSOR , reinterpret_cast<LONG>(cursor) );
}

sf::Cursor::~Cursor() { /*Nothing to do for destructor : no memory has been allocated (shared ressource principle)*/}
#elif defined(SFML_SYSTEM_LINUX)
sf::Cursor::Cursor(const sf::Cursor::Type t) {
	display = XOpenDisplay( NULL );

	switch( t ) {
		case sf::Cursor::Wait:
			cursor = XCreateFontCursor( display , XC_watch );
			break;
		case sf::Cursor::Hand:
			cursor = XCreateFontCursor( display , XC_hand1 );
			break;
		case sf::Cursor::Normal:
			cursor = XCreateFontCursor( display , XC_left_ptr );
			break;
		case sf::Cursor::Text:
			cursor = XCreateFontCursor( display , XC_xterm );
			break;
		   // For more cursor options on Linux go here: http://www.tronche.com/gui/x/xlib/appendix/b/
	}
}

void sf::Cursor::set(const sf::Window& Window) const {
	XDefineCursor( display , Window.getSystemHandle() , cursor );
	XFlush( display );
}

sf::Cursor::~Cursor() {
	XFreeCursor( display , cursor );
	delete display;
	display = NULL;
}
#else
	#error This OS is not yet supported by the cursor library.
#endif
