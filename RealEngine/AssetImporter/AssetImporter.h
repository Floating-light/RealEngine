#pragma once 

// #include <wstring>
#include <string>
#include <memory>

class RPrimitiveObject;
class RModelData;
class RAssetImporter
{
public:
    static std::shared_ptr<RPrimitiveObject> ImportModel(const std::wstring& InFile); 
    static std::shared_ptr<RModelData> ImportModelNew(const std::string& InFile);
};