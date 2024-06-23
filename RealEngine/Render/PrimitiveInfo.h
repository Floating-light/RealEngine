#pragma once 
#include <vector>
#include <string>
#include <memory>

class RModelData;

struct RPrimitiveObject
{
public:
    void IntializeBuffer();
    void SetModelData(std::shared_ptr<RModelData> InData, const std::string& Name);
private:
    std::string m_Name;
    std::shared_ptr<RModelData> m_ModelData; 
};