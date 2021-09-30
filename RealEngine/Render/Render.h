#pragma once 

class RRenderer
{
public:
    static RRenderer& Get();
    void Init();
    void OnUpdate();
};