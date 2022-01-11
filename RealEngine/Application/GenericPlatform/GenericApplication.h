#pragma once
#include <memory>
#include "GenericWindow.h"
#include "GenericApplicationMessageHandler.h"

class RGenericApplication
{
public:
    virtual std::shared_ptr<RGenericWindow> MakeWindow() {return nullptr;};    
    virtual void InitlializeWindow(std::shared_ptr<RGenericWindow> InWindow,const GenericWindowDesc& desc) {} ;
    void SetMessageHandler(const std::shared_ptr<GenericApplicationMessageHandler>& InHandler);
private:
    std::shared_ptr<GenericApplicationMessageHandler> MessageHandler;
};

inline void RGenericApplication::SetMessageHandler(const std::shared_ptr<GenericApplicationMessageHandler>& InHandler)
{
    MessageHandler = InHandler;
}
