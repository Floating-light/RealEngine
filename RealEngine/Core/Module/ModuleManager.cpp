#include "ModuleManager.h"
#include "Logging/Logging.h"

RModuleManager& RModuleManager::Get()
{
    static RModuleManager Singleton;
    return Singleton;
}

IModuleInterface* RModuleManager::GetModule(const std::string& Name)
{
    auto InsItr = InstancedModule.find(Name);
    if(InsItr != InstancedModule.end())
    {
        return InsItr->second;
    }
    auto Itr = m_moduleFactoryMap.find(Name);
    if(Itr != m_moduleFactoryMap.end())
    {
        return Itr->second();
    }
    RLOG(Error,"Cannot find moudle {}", Name);
    return nullptr;
}

