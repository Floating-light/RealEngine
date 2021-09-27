#pragma once
#include <memory>

class GenericApplication
{
public:
    std::shared_ptr<class GenericWindow> MakeWindow();    
};