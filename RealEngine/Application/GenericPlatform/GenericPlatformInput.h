#pragma once 
#include <unordered_map>
#include <string>

class GenericPlatformInput
{
public:
    static void GetKeyMap(std::unordered_map<int, std::string>& KeyMap);
    static void GetCharKeyMap(std::unordered_map<int, std::string>& CharKeyMap);
};