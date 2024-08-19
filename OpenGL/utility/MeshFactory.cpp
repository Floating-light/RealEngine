#include "MeshFactory.h"
#include "utility/Mesh.h"
#include "utility/LightObject.h"
#include "utility/Camera.h"

std::shared_ptr<RMesh> RMeshFactory::CreateMesh()
{
	return nullptr;
}


void RMeshFactory::setupMaterialParam(std::shared_ptr<Material> Mat, Matrix4 ModelTransform, std::shared_ptr<RCamera> Camera, std::shared_ptr<RLightObject> Light)
{
	Mat->setOrAddVectorParameterValue("eyePos", Camera->Location);
	Mat->setOrAddScalarParameter("emissionV", 2);

	Mat->setOrAddMatrix4ParameterValue("model", ModelTransform);
	Mat->setOrAddMatrix4ParameterValue("view", Camera->GetViewTransform());
	Mat->setOrAddMatrix4ParameterValue("projection", Camera->GetPerspective());

	//Mat->setOrAddVectorParameterValue("InColor", Vector3D(0.828, 0.364, 0.0));
	Mat->setOrAddVectorParameterValue("pointlight.position", Light->GetWorldLocation());
	Mat->setOrAddVectorParameterValue("pointlight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
	Mat->setOrAddVectorParameterValue("pointlight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	Mat->setOrAddVectorParameterValue("pointlight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
	Mat->setOrAddScalarParameter("pointlight.constant", 1.0f);
	Mat->setOrAddScalarParameter("pointlight.linear", 0.09f);
	Mat->setOrAddScalarParameter("pointlight.quadratic", 0.032f);
}
