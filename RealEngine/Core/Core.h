#pragma once 

#include <cassert>
#include <codecvt>
#include "Logging.h"
#include "MathUtilityDefine.h"
#include "Vector.h"
#include "Rotator.h"
#include "Transform.h"
#include "Matrix4.h"
#include "Vector2D.h"
#include "MathUtility.h"

extern std::wstring ExecutableDir;

class RCoreGlobal
{
public:
    static bool IsRequireExit() ;
    static void SetRequireExit();
private:
    static bool bRequireExit;
};

class RUtility
{
    public:
    
    // Convert utf-16 wstring to utf-8 string
    static std::string WideStringToString(const std::wstring& InStr)
    {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.to_bytes(InStr);
    }

    // Convert utf-8 string to utf-16 wstring
    static std::wstring StringToWideString(const std::string& InStr)
    {
        return std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>{}.from_bytes(InStr);
    }

    // Convert wstring to string 
    static std::string  WstringToString(const std::wstring& InStr)
    {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>{}.to_bytes(InStr);
    }

    // Convert string to wstring
    static std::wstring StringToWstring(const std::string& InStr)
    {
        return std::wstring_convert<std::codecvt_utf8<wchar_t>,wchar_t>{}.from_bytes(InStr);
    }
};