#pragma once 
#include <string>
#include <vector>
#include <memory>

class RGenericWindow;

class RApplication
{
public:
    static RApplication* Get();
    void Initilization();
    std::shared_ptr<RGenericWindow> GetMainWindow();
    void ProcessInput();
private:
    std::shared_ptr<RGenericWindow> MainWindow;
    std::vector<std::shared_ptr<RGenericWindow>> Windows;
    std::shared_ptr<class RGenericApplication> PlatformApp;
};