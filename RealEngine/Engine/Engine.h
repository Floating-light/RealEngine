#pragma once 

class REngine
{
public:
    static REngine* Get();
    void OnInit();
    void OnUpdate();
    void OnDestoryed();
    
};
