//=============================================================================
//File Name: insert.inl
//Description: Holds definition for template function "insert" from File class
//Author: Tyler Veness
//Last Modified: 5/27/2012
//Version: 0.1
//=============================================================================

#ifndef INSERT_INL_
#define INSERT_INL_

template<typename T> void File::insert( T newchar ) {
	std::string temp = "";
	temp += newchar;

	if ( allSelected ) {
		clear();

		allSelected = false;
	}

	if ( cursorPos.x > input[cursorPos.y].length() ) {
		if ( input[cursorPos.y].length() > 0 )
			cursorPos.x = input[cursorPos.y].length() - 1;
		else
			cursorPos.x = 0;
	}
			
	for ( unsigned int index = 0 ; index < temp.size() ; index++ ) {
		if ( temp.substr( index , 1 ) != "\n" ) {
			input[cursorPos.y] = input[cursorPos.y].substr( 0 , cursorPos.x ) + temp.substr( index , 1 ) + input[cursorPos.y].substr( cursorPos.x , input[cursorPos.y].length() - cursorPos.x );
			cursorPos.x++;
		}
		else {
			insert( cursorPos.y + 1 , "" ); //add new line to file
			cursorPos.x = 0;
			cursorPos.y++;
		}
	}

}

#endif /* INSERT_INL_ */
