#pragma once
#include <memory>

class RGenericApplication
{
public:
    virtual std::shared_ptr<class RGenericWindow> MakeWindow();    
    virtual void InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const struct GenericWindowDesc& desc);
};