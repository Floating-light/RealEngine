#pragma once 
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "Core.h"
#include "GenericApplicationMessageHandler.h"

class RGenericWindow;
class RGenericApplication;
class IAppViewport;
class RApplication : public GenericApplicationMessageHandler
{
public:
    static void Create();
    static RApplication& Get();
    void Initilization();
    std::shared_ptr<RGenericWindow> GetMainWindow();
    void ProcessInput();
    void RegisterGameViewport(std::shared_ptr<IAppViewport> InViewport);
    void SetOnMainWindowClosed(const std::function<void()>& InFunc );
public:
    // ~ Begin GenericApplicationMessageHandler interface 
    virtual bool OnKeyDown(const int KeyCode, const int CharacterCode, const bool IsRepeat)override;
    virtual bool OnKeyUp(const int KeyCode, const int CharacterCode, const bool IsRepeat)override;
    virtual bool OnMouseButtonDown(const std::shared_ptr<RGenericWindow>& Window, const EMouseButton Button, const Vector2D CursorPos)override;
    virtual bool OnMouseButtonUp(const EMouseButton Button, const Vector2D CursorPos)override;
    virtual bool OnRawMouseMove(const int X, const int Y)override;
    virtual void OnWindowClose(const std::shared_ptr<RGenericWindow>& Window) override;
    // ~ End GenericApplicationMessageHandler interface
private:
    std::shared_ptr<RGenericWindow> MainWindow;
    std::vector<std::shared_ptr<RGenericWindow>> Windows;
    std::shared_ptr<RGenericApplication> PlatformApp;

    std::shared_ptr<IAppViewport> GameViewport;

    static std::shared_ptr<RApplication> Application;
    std::function<void()> OnMainWindowsClosedFunc;
};