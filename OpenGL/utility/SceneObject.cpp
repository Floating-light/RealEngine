#include "SceneObject.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

RSceneObject::RSceneObject(std::vector<float>&& V) :
	VAO(0),
	VBO(0),
	Rotator(0.0f, 0.0f, 0.0f),
	Scale(1.0f, 1.0f, 1.0f),
	Translate(0.0f, 0.0f, 0.0f)
{
	Vertices = std::move(V);
}

glm::mat4 RSceneObject::GetModelTrasform()
{
	glm::mat4 res(1.0f);

	// translate 
	res = glm::translate(res, Translate);
	
	// rotation
	res = glm::rotate(res, glm::radians(Rotator.x), glm::vec3(1.0f, 0.0f, 0.0f));
	res = glm::rotate(res, glm::radians(Rotator.y), glm::vec3(0.0f, 1.0f, 0.0f));
	res = glm::rotate(res, glm::radians(Rotator.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// scale
	res = glm::scale(res, Scale);
	
	return std::move(res);
}

Vector3D RSceneObject::GetWorldLocation()
{
	return Translate;
}


void RSceneObject::ResetTransform()
{
	Rotator = glm::vec3(0.0f, 0.0f, 0.0f);
	Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	Translate = glm::vec3(0.0f, 0.0f, 0.0f);
}

void RSceneObject::SetModel(std::shared_ptr<RModel> NewModel)
{
	Model = NewModel;
}

void RSceneObject::InputEvent(int key, int action)
{
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		Translate.x += 0.2;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		Translate.x -= 0.2;

	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		Rotator.x += 10;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		Rotator.y += 10;
	}
}
void RSceneObject::Tick(float DeltaTime)
{
	//const float CurrentTime = glfwGetTime();
	//Scale = glm::vec3(1.0f, 1.0f, 1.0f) * abs(sinf(CurrentTime));
	//const int index = rand() % 20;
	//const int sign = rand() % 2;
	//if(index < 3)
	//	Rotator[index] += ((sign == 1) ? 1 : 1) * (rand() % 30);
}









RSceneObject RSceneObject::Create3DCube()
{
	return RSceneObject({ 
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		});
}

void RSceneObject::CreateVAO(RSceneObject& Obj)
{
	// Bind VAO
	glGenVertexArrays(1, &Obj.VAO);
	glBindVertexArray(Obj.VAO);

	// Create and Bind VBO
	glGenBuffers(1, &Obj.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, Obj.VBO);
	glBufferData(GL_ARRAY_BUFFER, Obj.Vertices.size() * sizeof(float), Obj.Vertices.data(), GL_STATIC_DRAW);

	// Set attribute pointer and enable
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// UnBind --> VBO --> VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}