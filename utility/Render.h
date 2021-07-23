#pragma once
#include <vector>
#include <memory>

class RSceneObject;
class RLightObject;
class RCamera;

class RRenderer
{
public:
	void Render(std::vector<std::shared_ptr<RSceneObject>>& Objects);
	void SetCamera(std::shared_ptr<RCamera> NewCamrea);
	void SetLight(std::shared_ptr < RLightObject> NewLight);
private:
	void DepthPass();

	std::shared_ptr<RCamera> Camera;
	std::shared_ptr<RLightObject> Light;
};