#include "InputKeyManager.h"
#include "Core.h"

std::shared_ptr<RInputKeyManager> RInputKeyManager::Instance = nullptr;

RInputKeyManager& RInputKeyManager::Get()
{
    if(!Instance)
    {
        Instance = std::make_shared<RInputKeyManager>();
    }
    return *Instance;
}

RKey RInputKeyManager::GetKeyFromCodes(const int KeyCode, const int CharCode) const 
{
    auto Itr = KeyMapVirtualToEnum.find(KeyCode);
    if(Itr == KeyMapVirtualToEnum.end())
    {
        Itr = KeyMapCharToEnum.find(CharCode);
    }
    if(Itr == KeyMapCharToEnum.end()) //  ? 
    {
        RLOG(ERROR) << "Can not find a RKey of KeyCode [" << KeyCode << "], Or CharCode : [" << CharCode << "]" ;
        return RKey::Invalid;
    }
    return Itr->second;
}

// will recall when language changed.
void RInputKeyManager::InitKeyMappings()
{

}