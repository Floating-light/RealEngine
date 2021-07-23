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
	// ����������
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

	// ������
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
	// �����Ǵ����� buffer �󶨵� OpenGL ָ���� VBO��buffer type
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// �˺�, ���ж�GL_ARRAY_BUFFER�Ĳ���������VBO
	if (v == nullptr)
	{
		//v = GetVertices();
	}
	// ����������copy ��GL_ARRAY_BUFFER bind ��buffer(VBO)
	// 
	glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

	// ����GL ��ν� GL_ARRAY_BUFFER �еĶ������ݵ����Ժ�vertex shader ���������ݹ�������
	/*
	* ���õ�ǰGL_ARRAY_BUFFER �󶨵�VBO�����ݵĽ�����ʽ
	* @param1 ��Ҫ���õĵڼ���vertex����, ��vert shader �е� layout location = 0, ��Ӧ
	* @param2 ������Ե�����, ��3��ֵ, vec3 ������ֵ
	* @param3 ÿ��ֵ������, float, vec3 ��ÿ����������Ͷ���float
	* @param4 �Ƿ�normalized��Щ����, ���ΪGL_TRUE, �������Integer(int, byte), ��תΪfloatʱ, integer �ᱻnormalizedΪ-1(���Ϊsigned data), 0, 1.
	* @param5 stride(����), ����ͬһ�����͵�vertex Attribute ֮���space. �������������������, ��֮��û��������϶(��������»���������Ϊ0, ��gl �Լ�����),Ҫע���ڴ沼��.
	* @param6 ������buffer ��, ÿ������һ�������е�ƫ��
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// enable ָ��location ��vertex attribute
	glEnableVertexAttribArray(0);
}

// �����ú�����Ҫ�õ�VBO, attributes pointers ΪVAO, Ȼ�󱣴���ЩVAO, ��������. 
void ConfigVertexArrayObejcts(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO)
{
	//float* vertices = GetVertices(); // �޷���������������, �ᱻ����һ����.
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

	// ��unbind VAO֮ǰ������unbind EBO
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

	// ��unbind VAO֮ǰ������unbind EBO
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

	// ��unbind VAO֮ǰ������unbind EBO
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

	// ����ָ��window ��framebuffer size ��callback.
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
		// ����ָ��window ������
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderPrograme);

		glBindVertexArray(VAO);
		//ConfigVertexBufferObjects();
		/*
		* @prama1 primitive type
		* @prama2 ��ʼ����vertex array �Ŀ�ʼ����, 
		* @prama3 �뻭���ٸ�����
		*/
		glDrawArrays(GL_TRIANGLES, 0, 6);

		/*
		* @param1 primitive type
		* @param2 elements ����, ��indices ����, ��Ҫ���Ķ�������
		* @param3 indices Ԫ������
		* @param4 ��ʼԪ��ƫ��
		*/
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glUseProgram(shaderPrograme_yellowfrag);

		// Exercises 2
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);


		glfwPollEvents(); // keyboard input, mouse movement events
		glfwSwapBuffers(window); // ����buffer, ��ʾ���µ���Ⱦ���
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderPrograme);

	glfwTerminate();

	return 0;
}