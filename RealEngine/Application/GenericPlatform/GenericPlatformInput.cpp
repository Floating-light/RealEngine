#include "GenericPlatformInput.h"

#include "Core.h"
#include "windows.h"

void GenericPlatformInput::GetKeyMap(std::unordered_map<int, std::string>& KeyMap)
{
#define ADDKEYMAP(KeyCode, KeyName)		KeyMap.emplace(static_cast<int>(KeyCode), std::string(KeyName))
    ADDKEYMAP( VK_LBUTTON, "LeftMouseButton") ;
	ADDKEYMAP( VK_RBUTTON, "RightMouseButton") ;
	ADDKEYMAP( VK_MBUTTON, "MiddleMouseButton") ;
	ADDKEYMAP( VK_XBUTTON1, "ThumbMouseButton") ;
	ADDKEYMAP( VK_XBUTTON2, "ThumbMouseButton2") ;
	ADDKEYMAP( VK_BACK, "BackSpace") ;
	ADDKEYMAP( VK_TAB, "Tab") ;
	ADDKEYMAP( VK_RETURN, "Enter") ;
	ADDKEYMAP( VK_PAUSE, "Pause") ;
	ADDKEYMAP( VK_CAPITAL, "CapsLock") ;
	ADDKEYMAP( VK_ESCAPE, "Escape") ;
	ADDKEYMAP( VK_SPACE, "SpaceBar") ;
	ADDKEYMAP( VK_PRIOR, "PageUp") ;
	ADDKEYMAP( VK_NEXT, "PageDown") ;
	ADDKEYMAP( VK_END, "End") ;
	ADDKEYMAP( VK_HOME, "Home") ;
	ADDKEYMAP( VK_LEFT, "Left") ;
	ADDKEYMAP( VK_UP, "Up") ;
	ADDKEYMAP( VK_RIGHT, "Right") ;
	ADDKEYMAP( VK_DOWN, "Down") ;
	ADDKEYMAP( VK_INSERT, "Insert") ;
	ADDKEYMAP( VK_DELETE, "Delete") ;
	ADDKEYMAP( VK_NUMPAD0, "NumPadZero") ;
	ADDKEYMAP( VK_NUMPAD1, "NumPadOne") ;
	ADDKEYMAP( VK_NUMPAD2, "NumPadTwo") ;
	ADDKEYMAP( VK_NUMPAD3, "NumPadThree") ;
	ADDKEYMAP( VK_NUMPAD4, "NumPadFour") ;
	ADDKEYMAP( VK_NUMPAD5, "NumPadFive") ;
	ADDKEYMAP( VK_NUMPAD6, "NumPadSix") ;
	ADDKEYMAP( VK_NUMPAD7, "NumPadSeven") ;
	ADDKEYMAP( VK_NUMPAD8, "NumPadEight") ;
	ADDKEYMAP( VK_NUMPAD9, "NumPadNine") ;
	ADDKEYMAP( VK_MULTIPLY, "Multiply") ;
	ADDKEYMAP( VK_ADD, "Add") ;
	ADDKEYMAP( VK_SUBTRACT, "Subtract") ;
	ADDKEYMAP( VK_DECIMAL, "Decimal") ;
	ADDKEYMAP( VK_DIVIDE, "Divide") ;
	ADDKEYMAP( VK_F1, "F1") ;
	ADDKEYMAP( VK_F2, "F2") ;
	ADDKEYMAP( VK_F3, "F3") ;
	ADDKEYMAP( VK_F4, "F4") ;
	ADDKEYMAP( VK_F5, "F5") ;
	ADDKEYMAP( VK_F6, "F6") ;
	ADDKEYMAP( VK_F7, "F7") ;
	ADDKEYMAP( VK_F8, "F8") ;
	ADDKEYMAP( VK_F9, "F9") ;
	ADDKEYMAP( VK_F10, "F10") ;
	ADDKEYMAP( VK_F11, "F11") ;
	ADDKEYMAP( VK_F12, "F12") ;
	ADDKEYMAP( VK_NUMLOCK, "NumLock") ;
	ADDKEYMAP( VK_SCROLL, "ScrollLock") ;
	ADDKEYMAP( VK_LSHIFT, "LeftShift") ;
	ADDKEYMAP( VK_RSHIFT, "RightShift") ;
	ADDKEYMAP( VK_LCONTROL, "LeftControl") ;
	ADDKEYMAP( VK_RCONTROL, "RightControl") ;
	ADDKEYMAP( VK_LMENU, "LeftAlt") ;
	ADDKEYMAP( VK_RMENU, "RightAlt") ;
	ADDKEYMAP( VK_LWIN, "LeftCommand") ;
	ADDKEYMAP( VK_RWIN, "RightCommand") ;
	
	GetCharKeyMap(KeyMap);
#undef ADDKEYMAP

}
void GenericPlatformInput::GetCharKeyMap(std::unordered_map<int, std::string>& CharKeyMap)
{
#define ADDKEYMAP(KeyCode, KeyName)		CharKeyMap.emplace(static_cast<int>(KeyCode), std::string(KeyName))

	ADDKEYMAP( '0', "Zero");
	ADDKEYMAP( '1', "One");
	ADDKEYMAP( '2', "Two");
	ADDKEYMAP( '3', "Three");
	ADDKEYMAP( '4', "Four");
	ADDKEYMAP( '5', "Five");
	ADDKEYMAP( '6', "Six");
	ADDKEYMAP( '7', "Seven");
	ADDKEYMAP( '8', "Eight");
	ADDKEYMAP( '9', "Nine");

	ADDKEYMAP( 'A', "A");
	ADDKEYMAP( 'B', "B");
	ADDKEYMAP( 'C', "C");
	ADDKEYMAP( 'D', "D");
	ADDKEYMAP( 'E', "E");
	ADDKEYMAP( 'F', "F");
	ADDKEYMAP( 'G', "G");
	ADDKEYMAP( 'H', "H");
	ADDKEYMAP( 'I', "I");
	ADDKEYMAP( 'J', "J");
	ADDKEYMAP( 'K', "K");
	ADDKEYMAP( 'L', "L");
	ADDKEYMAP( 'M', "M");
	ADDKEYMAP( 'N', "N");
	ADDKEYMAP( 'O', "O");
	ADDKEYMAP( 'P', "P");
	ADDKEYMAP( 'Q', "Q");
	ADDKEYMAP( 'R', "R");
	ADDKEYMAP( 'S', "S");
	ADDKEYMAP( 'T', "T");
	ADDKEYMAP( 'U', "U");
	ADDKEYMAP( 'V', "V");
	ADDKEYMAP( 'W', "W");
	ADDKEYMAP( 'X', "X");
	ADDKEYMAP( 'Y', "Y");
	ADDKEYMAP( 'Z', "Z");

	ADDKEYMAP( ';', "Semicolon");
	ADDKEYMAP( '=', "Equals");
	ADDKEYMAP( ',', "Comma");
	ADDKEYMAP( '-', "Hyphen");
	ADDKEYMAP( '.', "Period");
	ADDKEYMAP( '/', "Slash");
	ADDKEYMAP( '`', "Tilde");
	ADDKEYMAP( '[', "LeftBracket");
	ADDKEYMAP( '\\',"Backslash");
	ADDKEYMAP( ']', "RightBracket");
	ADDKEYMAP( '\'',"Apostrophe");
	ADDKEYMAP( ' ', "SpaceBar");
	ADDKEYMAP( '&', "Ampersand");
	ADDKEYMAP( '*', "Asterix");
	ADDKEYMAP( '^', "Caret");
	ADDKEYMAP( ':', "Colon");
	ADDKEYMAP( '$', "Dollar");
	ADDKEYMAP( '!', "Exclamation");
	ADDKEYMAP( '(', "LeftParantheses");
	ADDKEYMAP( ')', "RightParantheses");
	ADDKEYMAP( '"', "Quote");
	ADDKEYMAP( '_', "Underscore");
	ADDKEYMAP( 224, "A_AccentGrave");
	ADDKEYMAP( 231, "C_Cedille");
	ADDKEYMAP( 233, "E_AccentAigu");
	ADDKEYMAP( 232, "E_AccentGrave");
	ADDKEYMAP( 167, "Section");

#undef ADDKEYMAP
}