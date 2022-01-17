#pragma once 

class GenericPlatformInput
{
public:
    static int GetKeyMap(int* KeyCodes, std::string * KeyNames, int MaxMappings);
};