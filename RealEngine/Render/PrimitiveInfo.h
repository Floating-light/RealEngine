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
    RModelData* GetModelData()const { return m_ModelData.get(); } 
private:
    // Transofrm
    std::string m_Name;
    std::shared_ptr<RModelData> m_ModelData; 
};