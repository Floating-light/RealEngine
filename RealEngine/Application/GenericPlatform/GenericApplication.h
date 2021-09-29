#pragma once
#include <memory>
#include "GenericWindow.h"

class RGenericApplication
{
public:
    virtual std::shared_ptr<RGenericWindow> MakeWindow() {return nullptr;};    
    virtual void InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const GenericWindowDesc& desc) {} ;
};