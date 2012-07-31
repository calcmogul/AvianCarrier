#ifndef CURSOR_HPP_INCLUDED
#define CURSOR_HPP_INCLUDED

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#ifdef SFML_SYSTEM_WINDOWS
	#include <windows.h>
#elif defined(SFML_SYSTEM_LINUX)
	#include <X11/cursorfont.h>
	#include <X11/Xlib.h>
#else
	#error This OS is not yet supported by the cursor library.
#endif

#ifdef SFML_SYSTEM_WINDOWS

namespace sf {
	class Cursor {
		private:
			HCURSOR cursor; // type of cursor with Windows

		public:
			enum Type { Wait , Text , Normal , Hand };
			Cursor( const Type t );
			virtual ~Cursor();

			void set( const sf::Window& Window ) const;
	};
}

#else

namespace sf {
	class Cursor {
		private:
			XID cursor;
			Display* display;

		public:
			enum Type { Wait , Text , Normal , Hand };
			Cursor( const Type t );
			virtual ~Cursor();

			void set( const sf::Window& Window ) const;
	};
}

#endif

#endif // CURSOR_HPP_INCLUDED
