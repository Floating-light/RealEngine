#pragma once 
#include <unordered_map>
#include <memory>

#include "InputTypes.h"
class RInputKeyManager
{
public:
    static RInputKeyManager& Get();
    RKey GetKeyFromCodes(const int KeyCode, const int CharCode) const ;

    // will recall when language changed.
    void InitKeyMappings();
private:
    RInputKeyManager() 
    {
        InitKeyMappings();
    };
    static std::shared_ptr<RInputKeyManager> Instance;
    std::unordered_map<int, RKey> KeyMapVirtualToKey;
    std::unordered_map<int, RKey> KeyMapCharToKey;
};