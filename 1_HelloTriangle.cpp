#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <vector>
#include <string>


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

enum class InputVertexID
{
	TriangleCenter,
	TriangleLeft,
	TriangleRight,
	Rectangle
};

// Get The vertices to rendering
std::vector<float>& GetVertices(const InputVertexID Type = InputVertexID::TriangleCenter)
{
	// 单个三角形
	static std::vector<float > VertTriangleCenter = {
			 -0.5f, -0.5f, 0.0f,
			  0.5f, -0.5f, 0.0f,
			  0.0f, 0.5f, 0.0f };

	static std::vector<float > VertTriangleLeft = {
			 -1.f, -0.5f, 0.0f,
			 -0.5f, 0.5f, 0.0f,
			  0.0f, -0.5f, 0.0f };

	static std::vector<float > VertTriangleRight = {
		  1.f, -0.5f, 0.0f,
		  0.5f, 0.5f, 0.0f,
		  0.0f, -0.5f, 0.0f };

	// 正方形
	static std::vector<float > VertRectangle = { 0.5f, 0.5f, 0.0f, 
											  0.5f, -0.5f, 0.0f, 
											  - 0.5f, -0.5f, 0.0f,
											  -0.5f, 0.5f, 0.0f };
	if (Type == InputVertexID::TriangleCenter)
	{
		return VertTriangleCenter;
	}
	else if (Type == InputVertexID::TriangleLeft)
	{
		return VertTriangleLeft;
	}
	else if (Type == InputVertexID::TriangleRight)
	{
		return VertTriangleRight;
	}
	else if (Type == InputVertexID::Rectangle)
	{
		return VertRectangle;
	}
	return VertTriangleCenter;
}

std::vector<unsigned int>& GetIndices()
{
	static std::vector<unsigned int> indices = { 0,1,3,
												1,2,3 };
	return indices;
}

// ABORT
void ConfigVertexBufferObjects(float *v = nullptr)
{
	unsigned int VBO;
	glGenBuffers(1, &VBO); // generate a buffer
	// the buffer type of vertex buffer object is GL_ARRAY_BUFFER
	// 将我们创建的 buffer 绑定到 OpenGL 指定的 VBO的buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 此后, 所有对GL_ARRAY_BUFFER的操作都将用VBO
	if (v == nullptr)
	{
		//v = GetVertices();
	}
	// 将顶点数据copy 到GL_ARRAY_BUFFER bind 的buffer(VBO)
	// 
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

	// 告诉GL 如何将 GL_ARRAY_BUFFER 中的顶点数据的属性和vertex shader 的输入数据关联起来
	/*
	* 设置当前GL_ARRAY_BUFFER 绑定的VBO的数据的解析方式
	* @param1 想要配置的第几个vertex属性, 和vert shader 中的 layout location = 0, 对应
	* @param2 这个属性的数量, 有3个值, vec3 有三个值
	* @param3 每个值的类型, float, vec3 的每个坐标的类型都是float
	* @param4 是否normalized这些数据, 如果为GL_TRUE, 且输入的Integer(int, byte), 在转为float时, integer 会被normalized为-1(如果为signed data), 0, 1.
	* @param5 stride(步长), 相邻同一个类型的vertex Attribute 之间的space. 这里仅隔了三个浮点数, 且之间没有其它空隙(这种情况下还可以设置为0, 让gl 自己决定),要注意内存布局.
	* @param6 数据在buffer 中, 每完整的一组数据中的偏移
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enable 指定location 的vertex attribute
	glEnableVertexAttribArray(0);
}

// 先配置好所有要用的VBO, attributes pointers 为VAO, 然后保存这些VAO, 后面再用. 
void ConfigVertexArrayObejcts(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO)
{
	//float* vertices = GetVertices(); // 无法返回完整的数组, 会被当成一个数.
	auto& vvector = GetVertices(InputVertexID::TriangleCenter);
	auto& indices = GetIndices();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vvector.size() * sizeof(float) , vvector.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, vvector.size() * sizeof(float) , vvector.data(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 在unbind VAO之前都不能unbind EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // error VAO also remenber the unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


// Draw use:glDrawArrays(GL_TRIANGLES, 0, 6); 
void ConfigVertexArrayObejcts_E1(unsigned int& VAO, unsigned int& VBO)
{
	static float vertices[] = { 
		  -1.f, -0.5f, 0.0f,
		  -0.5f, 0.5f, 0.0f,
		  0.0f, -0.5f, 0.0f,

		  1.f, -0.5f, 0.0f,
		  0.5f, 0.5f, 0.0f,
		  0.0f, -0.5f, 0.0f };

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, vvector.size() * sizeof(float) , vvector.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 在unbind VAO之前都不能unbind EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // error VAO also remenber the unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Draw use:glDrawArrays(GL_TRIANGLES, 0, 6); 
void ConfigVertexArrayObejct_Gen(unsigned int& VAO, unsigned int& VBO, InputVertexID type)
{
	auto& data = GetVertices(type);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size()* sizeof(float), data.data(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, vvector.size() * sizeof(float) , vvector.data(), GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 在unbind VAO之前都不能unbind EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // error VAO also remenber the unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int getCompiledVertShader()
{
	const char* vertexShaderSource = "#version 330 core \n"
									  "layout(location = 0) in vec3 aPos;\n"
									  "out vec4 vertexColor;"
									  "void main()\n"
									  "{\n"
									  "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
									  "vertexColor = normalize(gl_Position);"
									  "}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPOLATION_FAILED\n"
			<< infoLog << std::endl;
		glDeleteShader(vertexShader);
		return 0;
	}
	return vertexShader;
}

unsigned int getCompiledFragShader(float R, float G, float B, float A)
{
	const char* fragmentShaderSource = "#version 330 core\n"
										"out vec4 FragColor;\n"
										"in vec4 vertexColor;\n"
										"void main()\n"
										"{\n"
										"	FragColor = vertexColor;\n"
										"}\0";
	//std::string stdString = "#version 330 core\nout vec4 FragColor;\nin vec4 vertexColor;\nvoid main()\n{\nFragColor = vec4(" + std::to_string(R) + ", " + std::to_string(G) + ", " + std::to_string(B) + ", " + std::to_string(A) + ");\n}\0";
	std::string stdString = "#version 330 core\nout vec4 FragColor;\nin vec4 vertexColor;\nvoid main()\n{\nFragColor = vertexColor;\n}\0";
	const char* fragShaderShource = stdString.data();
	
		
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragShaderShource, nullptr);
	glCompileShader(fragmentShader);

	int success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
			<< infoLog << std::endl;
		glDeleteShader(fragmentShader);
		return 0;
	}
	return fragmentShader;
}

unsigned int  createShaderPrograme(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::LINK::LINK_FAILED\n"
			<< infoLog << std::endl;
		glDeleteShader(shaderProgram);
		return 0;
	}
	return shaderProgram;
}

int main()
{
	// Init glfw
	glfwInit();
	// Specify gl version 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Gl api version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a display window 
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); // 
		return -1;
	}
	// make a opengl context from previous created window
	glfwMakeContextCurrent(window);

	// load all opengl function by use the function provided by glfw via glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置指定window 的framebuffer size 的callback.
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//glViewport(0, 0, 800, 600);

	// get shader programe
	unsigned int vertexShader = getCompiledVertShader();
	unsigned int fragShader = getCompiledFragShader(1.0f, 0.5f, 0.2f, 1.0f);
	unsigned int shaderPrograme = createShaderPrograme(vertexShader, fragShader);
	glDeleteShader(fragShader);

	fragShader = getCompiledFragShader(1.0f, 1.0f, 0.0f, 1.0f);
	unsigned int shaderPrograme_yellowfrag = createShaderPrograme(vertexShader, fragShader);
	glDeleteShader(fragShader);
	glDeleteShader(vertexShader);

	// vertex array object, vertex buffer object, element buffer object
	unsigned int VAO, VBO, EBO;
	//ConfigVertexArrayObejcts(VAO, VBO, EBO);

	// Exercises 1
	// Draw with glDrawArrays(GL_TRIANGLES, 0, 6)
	// ConfigVertexArrayObejcts_E1(VAO, VBO);

	// Exercises 2
	ConfigVertexArrayObejct_Gen(VAO, VBO, InputVertexID::TriangleLeft);
	unsigned int VAO2, VBO2;
	ConfigVertexArrayObejct_Gen(VAO2, VBO2, InputVertexID::TriangleRight);


	// wireframe rendering
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// 处理指定window 的输入
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderPrograme);

		glBindVertexArray(VAO);
		//ConfigVertexBufferObjects();
		/*
		* @prama1 primitive type
		* @prama2 开始画的vertex array 的开始索引, 
		* @prama3 想画多少个顶点
		*/
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*
		* @param1 primitive type
		* @param2 elements 数量, 即indices 数量, 即要画的顶点数量
		* @param3 indices 元素类型
		* @param4 开始元素偏移
		*/
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glUseProgram(shaderPrograme_yellowfrag);

		// Exercises 2
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		glfwPollEvents(); // keyboard input, mouse movement events
		glfwSwapBuffers(window); // 交换buffer, 显示最新的渲染结果
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderPrograme);

	glfwTerminate();

	return 0;
}