#pragma once 
#include <string>
#include <unordered_map>
#include <functional>
class IModuleInterface
{
public:
    virtual ~IModuleInterface(){}
    virtual void InitializeModule()
    {

    }
    virtual void DeInitializeModule() 
    {

    }
};

class RModuleManager
{
public:
    using ModuleFactoryFunc = std::function<IModuleInterface*(void)>;
    using ModuleFactoryMap = std::unordered_map<std::wstring, ModuleFactoryFunc>;
    RModuleManager& Get();
    void RegisterStaticLinkedModule(const std::wstring& ModuleName, const ModuleFactoryFunc& Factory)
    {
        m_moduleFactoryMap.emplace(ModuleName, Factory);
    }
    IModuleInterface* GetModule(const std::wstring& Name);
    ModuleFactoryMap m_moduleFactoryMap;
private:
    RModuleManager(){}
};

template<class T>
struct RMoudleInterfaceRegisterant
{
public:
    RMoudleInterfaceRegisterant(const std::wstring& ModuleName)
    {
        RModuleManager::Get().RegisterStaticLinkedModule(ModuleName, 
        []()
        {
            return new T;
        });
    }
};

#define RDEFINE_MOUDLE(MoudleClass, MoudleName) \
    RMoudleInterfaceRegisterant<Moudle> ModuleRegestrant_##MoudleName;\
    extern "C" void ForceStaticLibraryReference() { }
