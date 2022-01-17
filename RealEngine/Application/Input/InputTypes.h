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
    std::string ToString() const 
    {
        return std::string(KeyName);
    }
    static const RKey Invalid;
private: 
    std::string_view KeyName;
};