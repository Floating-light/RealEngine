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

// https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok
// https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok/11336506#11336506
// https://github.com/zoolib/zoolib_cxx/blob/6e52b8ac85c6636d70f14b4c8123ff67e5dc5837/Core/zoolib/Compat_MSVCStaticLib.h
// https://stackoverflow.com/questions/2991927/how-to-force-inclusion-of-an-object-file-in-a-static-library-when-linking-into-e
class RModuleManager
{
public:
    using ModuleFactoryFunc = std::function<IModuleInterface*(void)>;
    using ModuleFactoryMap = std::unordered_map<std::string, ModuleFactoryFunc>;
    static RModuleManager& Get();
    void RegisterStaticLinkedModule(const std::string& ModuleName, const ModuleFactoryFunc& Factory)
    {
        m_moduleFactoryMap.emplace(ModuleName, Factory);
    }
    IModuleInterface* GetModule(const std::string& Name);
    template<class T>
    static T* GetModule(const std::string& Name)
    {
        return static_cast<T*>(RModuleManager::Get().GetModule(Name));
    }
    ModuleFactoryMap m_moduleFactoryMap;
    std::unordered_map<std::string, IModuleInterface*> InstancedModule;
private:
    RModuleManager(){}
};

template<class T>
struct RMoudleInterfaceRegisterant
{
public:
    RMoudleInterfaceRegisterant(const std::string& ModuleName)
    {
        RModuleManager::Get().RegisterStaticLinkedModule(ModuleName, 
        []()
        {
            return new T;
        });
    }
};

#define RDEFINE_MOUDLE(MoudleClass, MoudleName) \
    RMoudleInterfaceRegisterant<MoudleClass> ModuleRegestrant_##MoudleName(#MoudleName);\
    extern "C" void ForceStaticLibraryReference_##MoudleName() { }
