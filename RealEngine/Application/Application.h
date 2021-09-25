#pragma once 
#include <string>
#include <vector>
#include <memory>

class RWindow;

class RApplication
{
public:
    static RApplication& Get();
    void Initilization();
    std::shared_ptr<RWindow> GetMainWindow();
    std::vector<std::shared_ptr<class RWindow>> Windows;
};