#include "ModuleManager.h"
#include "Logging/Logging.h"

RModuleManager& RModuleManager::Get()
{
    static RModuleManager Singleton;
    return Singleton;
}

IModuleInterface* RModuleManager::GetModule(const std::wstring& Name)
{
    auto Itr = m_moduleFactoryMap.find(Name);
    if(Itr != m_moduleFactoryMap.end())
    {
        return Itr->second();
    }
    RLOG(ERROR) << "Cannot find moudle " << Name << std::endl;
    return nullptr;
}

