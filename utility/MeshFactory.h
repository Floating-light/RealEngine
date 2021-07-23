#pragma once 
#include <memory>
#include "utility/CoreType.h"

class RMesh;
class Material;
class RCamera;
class RLightObject;
class RMeshFactory
{
public:
	static std::shared_ptr<RMesh> CreateMesh();
	static void setupMaterialParam(std::shared_ptr<Material> Mat,Matrix4 ModelTransform, std::shared_ptr<RCamera> Camera, std::shared_ptr<RLightObject> Light);
};