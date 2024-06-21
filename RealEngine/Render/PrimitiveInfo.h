#pragma once 
#include <vector>
#include <string>
#include <memory>

class RModelData;

struct RPrimitiveObject
{
public:
    std::string mName;
    void IntializeBuffer();
    std::shared_ptr<RModelData> m_ModelData; 
};