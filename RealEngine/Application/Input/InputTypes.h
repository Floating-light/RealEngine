#pragma once 
#include <string>
struct RKey
{
public:
    RKey()
    {

    }

    RKey(const std::string InName)
        : KeyName(InName)
    {

    }
	bool operator==(const RKey& InKey) const 
	{
		return KeyName == InKey.KeyName;
	}
	bool operator!=(const RKey& InKey) const 
	{
		return !(*this == InKey);
	}
    std::string ToString() const 
    {
        return std::string(KeyName);
    }
    static const RKey Invalid;
    static const RKey AnyKey;

	static const RKey MouseX;
	static const RKey MouseY;
	static const RKey Mouse2D;
	static const RKey MouseScrollUp;
	static const RKey MouseScrollDown;
	static const RKey MouseWheelAxis;

	static const RKey LeftMouseButton;
	static const RKey RightMouseButton;
	static const RKey MiddleMouseButton;
	static const RKey ThumbMouseButton;
	static const RKey ThumbMouseButton2;

	static const RKey BackSpace;
	static const RKey Tab;
	static const RKey Enter;
	static const RKey Pause;

	static const RKey CapsLock;
	static const RKey Escape;
	static const RKey SpaceBar;
	static const RKey PageUp;
	static const RKey PageDown;
	static const RKey End;
	static const RKey Home;

	static const RKey Left;
	static const RKey Up;
	static const RKey Right;
	static const RKey Down;

	static const RKey Insert;
	static const RKey Delete;

	static const RKey Zero;
	static const RKey One;
	static const RKey Two;
	static const RKey Three;
	static const RKey Four;
	static const RKey Five;
	static const RKey Six;
	static const RKey Seven;
	static const RKey Eight;
	static const RKey Nine;

	static const RKey A;
	static const RKey B;
	static const RKey C;
	static const RKey D;
	static const RKey E;
	static const RKey F;
	static const RKey G;
	static const RKey H;
	static const RKey I;
	static const RKey J;
	static const RKey K;
	static const RKey L;
	static const RKey M;
	static const RKey N;
	static const RKey O;
	static const RKey P;
	static const RKey Q;
	static const RKey R;
	static const RKey S;
	static const RKey T;
	static const RKey U;
	static const RKey V;
	static const RKey W;
	static const RKey X;
	static const RKey Y;
	static const RKey Z;

	static const RKey NumPadZero;
	static const RKey NumPadOne;
	static const RKey NumPadTwo;
	static const RKey NumPadThree;
	static const RKey NumPadFour;
	static const RKey NumPadFive;
	static const RKey NumPadSix;
	static const RKey NumPadSeven;
	static const RKey NumPadEight;
	static const RKey NumPadNine;

	static const RKey Multiply;
	static const RKey Add;
	static const RKey Subtract;
	static const RKey Decimal;
	static const RKey Divide;

	static const RKey F1;
	static const RKey F2;
	static const RKey F3;
	static const RKey F4;
	static const RKey F5;
	static const RKey F6;
	static const RKey F7;
	static const RKey F8;
	static const RKey F9;
	static const RKey F10;
	static const RKey F11;
	static const RKey F12;

	static const RKey NumLock;

	static const RKey ScrollLock;

	static const RKey LeftShift;
	static const RKey RightShift;
	static const RKey LeftControl;
	static const RKey RightControl;
	static const RKey LeftAlt;
	static const RKey RightAlt;
	static const RKey LeftCommand;
	static const RKey RightCommand;

	static const RKey Semicolon;
	static const RKey Equals;
	static const RKey Comma;
	static const RKey Underscore;
	static const RKey Hyphen;
	static const RKey Period;
	static const RKey Slash;
	static const RKey Tilde;
	static const RKey LeftBracket;
	static const RKey Backslash;
	static const RKey RightBracket;
	static const RKey Apostrophe;

	static const RKey Ampersand;
	static const RKey Asterix;
	static const RKey Caret;
	static const RKey Colon;
	static const RKey Dollar;
	static const RKey Exclamation;
	static const RKey LeftParantheses;
	static const RKey RightParantheses;
	static const RKey Quote;

	static const RKey A_AccentGrave;
	static const RKey E_AccentGrave;
	static const RKey E_AccentAigu;
	static const RKey C_Cedille;
	static const RKey Section;
private: 
    std::string KeyName;
};