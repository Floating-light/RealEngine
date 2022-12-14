#pragma once 

// #include <wstring>
#include <string>
#include <memory>

class RPrimitiveObject;
class RAssetImporter
{
public:
    static std::shared_ptr<RPrimitiveObject> ImportModel(const std::wstring& InFile);
};