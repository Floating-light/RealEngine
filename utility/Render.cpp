#include "Render.h"
#include "SceneObject.h"
#include "Model.h"
#include "utility/LightObject.h"
#include "utility/Camera.h"
#include "utility/MeshFactory.h"
#include <iostream>
void RRenderer::Render(std::vector<std::shared_ptr<RSceneObject>>& Objects)
{
	for(auto Obj : Objects)
	{
		Obj->GetModelTrasform();
		for (auto& mesh : Obj->Model->meshes)
		{
			auto MaterialPtr = mesh.GetMaterial();
			RMeshFactory::setupMaterialParam(MaterialPtr, Obj->GetModelTrasform(), Camera, Light);
			mesh.Draw(MaterialPtr);
		}
	}
}

void RRenderer::DepthPass()
{

}


void RRenderer::SetCamera(std::shared_ptr<RCamera> NewCamrea)
{
	Camera = NewCamrea;
}
void RRenderer::SetLight(std::shared_ptr < RLightObject> NewLight)
{
	Light = NewLight;
}



