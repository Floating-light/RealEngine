#include "InputKeyManager.h"
#include "InputTypes.h"
#include "Core.h"
#include "GenericPlatform/GenericPlatformInput.h"

const RKey RKey::Invalid("Invalid""Invalid");
const RKey RKey::AnyKey("AnyKey");
const RKey RKey::MouseX("MouseX");
const RKey RKey::MouseY("MouseY");
const RKey RKey::Mouse2D("Mouse2D");
const RKey RKey::MouseScrollUp("MouseScrollUp");
const RKey RKey::MouseScrollDown("MouseScrollDown");
const RKey RKey::MouseWheelAxis("MouseWheelAxis");
const RKey RKey::LeftMouseButton("LeftMouseButton");
const RKey RKey::RightMouseButton("RightMouseButton");
const RKey RKey::MiddleMouseButton("MiddleMouseButton");
const RKey RKey::ThumbMouseButton("ThumbMouseButton");
const RKey RKey::ThumbMouseButton2("ThumbMouseButton2");
const RKey RKey::BackSpace("BackSpace");
const RKey RKey::Tab("Tab");
const RKey RKey::Enter("Enter");
const RKey RKey::Pause("Pause");
const RKey RKey::CapsLock("CapsLock");
const RKey RKey::Escape("Escape");
const RKey RKey::SpaceBar("SpaceBar");
const RKey RKey::PageUp("PageUp");
const RKey RKey::PageDown("PageDown");
const RKey RKey::End("End");
const RKey RKey::Home("Home");
const RKey RKey::Left("Left");
const RKey RKey::Up("Up");
const RKey RKey::Right("Right");
const RKey RKey::Down("Down");
const RKey RKey::Insert("Insert");
const RKey RKey::Delete("Delete");
const RKey RKey::Zero("Zero");
const RKey RKey::One("One");
const RKey RKey::Two("Two");
const RKey RKey::Three("Three");
const RKey RKey::Four("Four");
const RKey RKey::Five("Five");
const RKey RKey::Six("Six");
const RKey RKey::Seven("Seven");
const RKey RKey::Eight("Eight");
const RKey RKey::Nine("Nine");
const RKey RKey::A("A");
const RKey RKey::B("B");
const RKey RKey::C("C");
const RKey RKey::D("D");
const RKey RKey::E("E");
const RKey RKey::F("F");
const RKey RKey::G("G");
const RKey RKey::H("H");
const RKey RKey::I("I");
const RKey RKey::J("J");
const RKey RKey::K("K");
const RKey RKey::L("L");
const RKey RKey::M("M");
const RKey RKey::N("N");
const RKey RKey::O("O");
const RKey RKey::P("P");
const RKey RKey::Q("Q");
const RKey RKey::R("R");
const RKey RKey::S("S");
const RKey RKey::T("T");
const RKey RKey::U("U");
const RKey RKey::V("V");
const RKey RKey::W("W");
const RKey RKey::X("X");
const RKey RKey::Y("Y");
const RKey RKey::Z("Z");
const RKey RKey::NumPadZero("NumPadZero");
const RKey RKey::NumPadOne("NumPadOne");
const RKey RKey::NumPadTwo("NumPadTwo");
const RKey RKey::NumPadThree("NumPadThree");
const RKey RKey::NumPadFour("NumPadFour");
const RKey RKey::NumPadFive("NumPadFive");
const RKey RKey::NumPadSix("NumPadSix");
const RKey RKey::NumPadSeven("NumPadSeven");
const RKey RKey::NumPadEight("NumPadEight");
const RKey RKey::NumPadNine("NumPadNine");
const RKey RKey::Multiply("Multiply");
const RKey RKey::Add("Add");
const RKey RKey::Subtract("Subtract");
const RKey RKey::Decimal("Decimal");
const RKey RKey::Divide("Divide");
const RKey RKey::F1("F1");
const RKey RKey::F2("F2");
const RKey RKey::F3("F3");
const RKey RKey::F4("F4");
const RKey RKey::F5("F5");
const RKey RKey::F6("F6");
const RKey RKey::F7("F7");
const RKey RKey::F8("F8");
const RKey RKey::F9("F9");
const RKey RKey::F10("F10");
const RKey RKey::F11("F11");
const RKey RKey::F12("F12");
const RKey RKey::NumLock("NumLock");
const RKey RKey::ScrollLock("ScrollLock");
const RKey RKey::LeftShift("LeftShift");
const RKey RKey::RightShift("RightShift");
const RKey RKey::LeftControl("LeftControl");
const RKey RKey::RightControl("RightControl");
const RKey RKey::LeftAlt("LeftAlt");
const RKey RKey::RightAlt("RightAlt");
const RKey RKey::LeftCommand("LeftCommand");
const RKey RKey::RightCommand("RightCommand");
const RKey RKey::Semicolon("Semicolon");
const RKey RKey::Equals("Equals");
const RKey RKey::Comma("Comma");
const RKey RKey::Underscore("Underscore");
const RKey RKey::Hyphen("Hyphen");
const RKey RKey::Period("Period");
const RKey RKey::Slash("Slash");
const RKey RKey::Tilde("Tilde");
const RKey RKey::LeftBracket("LeftBracket");
const RKey RKey::Backslash("Backslash");
const RKey RKey::RightBracket("RightBracket");
const RKey RKey::Apostrophe("Apostrophe");
const RKey RKey::Ampersand("Ampersand");
const RKey RKey::Asterix("Asterix");
const RKey RKey::Caret("Caret");
const RKey RKey::Colon("Colon");
const RKey RKey::Dollar("Dollar");
const RKey RKey::Exclamation("Exclamation");
const RKey RKey::LeftParantheses("LeftParantheses");
const RKey RKey::RightParantheses("RightParantheses");
const RKey RKey::Quote("Quote");
const RKey RKey::A_AccentGrave("A_AccentGrave");
const RKey RKey::E_AccentGrave("E_AccentGrave");
const RKey RKey::E_AccentAigu("E_AccentAigu");
const RKey RKey::C_Cedille("C_Cedille");
const RKey RKey::Section("Section");

std::shared_ptr<RInputKeyManager> RInputKeyManager::Instance = nullptr;

RInputKeyManager& RInputKeyManager::Get()
{
    if(!Instance)
    {
        Instance = std::shared_ptr<RInputKeyManager>(new RInputKeyManager());
    }
    return *Instance;
}

RKey RInputKeyManager::GetKeyFromCodes(const int KeyCode, const int CharCode) const 
{
    RKey Result;
    if(KeyMapVirtualToKey.find(KeyCode) != KeyMapVirtualToKey.end())
    {
        Result = KeyMapVirtualToKey.find(KeyCode)->second;
    }
    else if(KeyMapCharToKey.find(CharCode) != KeyMapCharToKey.end()) //  ? 
    {
        Result = KeyMapCharToKey.find(CharCode)->second;
    }
    else
    {
        RLOG(ERROR) << "Can not find a RKey of KeyCode [" << KeyCode << "], Or CharCode : [" << CharCode << "]" ;
        Result =  RKey::Invalid;
    }
    return Result;
}

// will recall when language changed.
void RInputKeyManager::InitKeyMappings()
{
    std::unordered_map<int, std::string> GotKeyMap;
    std::unordered_map<int, std::string> GotCharMap;
    GenericPlatformInput::GetKeyMap(GotKeyMap);
    GenericPlatformInput::GetCharKeyMap(GotCharMap);

    KeyMapVirtualToKey.clear();
    KeyMapCharToKey.clear();

    for(auto Itr = GotKeyMap.cbegin(); Itr != GotKeyMap.cend(); Itr++)
    {
        KeyMapVirtualToKey.emplace(Itr->first, RKey(Itr->second));
    }

    for(auto Itr = GotCharMap.cbegin(); Itr != GotCharMap.cend(); Itr++)
    {
        KeyMapCharToKey.emplace(Itr->first, RKey(Itr->second));
    }
}