#pragma once
#include <functional>
#include <iostream>
#include "glm/glm.hpp"

struct GLFWwindow;

using Vector3D = glm::vec3;
using Vector2D = glm::vec2;
using Matrix4 = glm::mat4;

// Called when Pressed or Release some key
using KeyEventCallback = std::function<void(int, int)>;
// Called every frame to test if some key is pressed
using KeyProcess = std::function<void(GLFWwindow*, float Rate)>;

using MouseCursorMoveCallBack = std::function<void(double, double)>;

using MouseScrollCallBack = std::function<void(double, double)>;

// https://codereview.stackexchange.com/questions/211957/implementing-a-logging-system-in-c17
// TODO: a more beautiful log system 
#define Log(CategoryName, Verbosity, Format)  std::cout << CategoryName << "::" << Verbosity << " ----->>> " << Format << std::endl;