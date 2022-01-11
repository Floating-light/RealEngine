#pragma once 
#include <string>
#include <vector>
#include <memory>

#include "Core.h"
#include "GenericApplicationMessageHandler.h"

class RGenericWindow;
class RGenericApplication;
class RApplication : public GenericApplicationMessageHandler
{
public:
    static void Create();
    static RApplication& Get();
    void Initilization();
    std::shared_ptr<RGenericWindow> GetMainWindow();
    void ProcessInput();
public:
    // ~ Begin GenericApplicationMessageHandler interface 
    virtual bool OnKeyDown(const int KeyCode, const int CharacterCode, const bool IsRepeat)override;
    virtual bool OnKeyUp(const int KeyCode, const int CharacterCode, const bool IsRepeat)override;
    virtual bool OnMouseButtonDown(const std::shared_ptr<RGenericWindow>& Window, const EMouseButton Button)override;
    virtual bool OnMouseButtonUp(const EMouseButton Button)override;
    virtual bool OnRawMouseMove(const int X, const int Y)override;
private:
    std::shared_ptr<RGenericWindow> MainWindow;
    std::vector<std::shared_ptr<RGenericWindow>> Windows;
    std::shared_ptr<RGenericApplication> PlatformApp;

    static std::shared_ptr<RApplication> Application;
};