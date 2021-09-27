#pragma once 
#include <string>

struct GenericWindowDesc
{
public:
    int width ;
    int height;
    std::wstring title;
};

class GenericWindow
{
public:
    virtual ~GenericWindow(){};
    virtual void Initialize(){};
    virtual void* GetWindowHandle(){return nullptr;};
    virtual void SetTitle(const std::wstring& Title);
};