#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
unsigned int constexpr Width = 1024;
unsigned int constexpr Height = 768;
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow* window);
//vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout(location=0) in vec3 aPos;\n"
"layout(location=1) in vec3 color;\n"
"out vec3 color_result;\n"
"void main()\n"
"{\n"
"gl_Position=vec4(aPos,1.0);\n"
"color_result=color;\n"
"}\0";
//fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 color_result;\n"
"out vec4 FragColor;\n"
"uniform vec4 breath;\n"
"void main()\n"
"{\n"
"FragColor=vec4(color_result,1.0f);\n"
"}\n\0";

int main()
{
	//初始化glfw
	glfwInit();
	//设置opengl版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//core profile选项
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//创建窗口对象
	GLFWwindow* mywindow = glfwCreateWindow(Width, Height, "opengl", NULL, NULL);
	if (mywindow == nullptr)
	{
		std::cout << "fail to create window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mywindow);
	//register frame_size_callback function
	glfwSetFramebufferSizeCallback(mywindow, framebuffer_size_callback);
	//initialize glad
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout<<"fail to initialize glad\n";
	}

	//create shader object
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//check if the shader is compiled successfully
	int success;
	char error_info[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, error_info);
		std::cout << "VERTEX SHADER COMPILATION FAILED\n"<<error_info<<"\n";
	}
	//create shader object
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER),fragmentShader1=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(fragmentShader);

	//check if the shader is compiled successfully
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, error_info);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << error_info << "\n";
	}

	//create shader program
	unsigned int shader_program = glCreateProgram();
	//first shader program
	glAttachShader(shader_program,vertexShader);
	glAttachShader(shader_program,fragmentShader);
	//
	glLinkProgram(shader_program);

	//check link status
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, error_info);
		std::cout << "SHADER PROGRAM LINKING FAILED\n"<<error_info<<"\n";
	}

	//delete shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//vertex data
	float vertex[] = {
		-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,
		0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,
		0.0f,0.5f,0.0f,0.0f,0.0f,1.0f
	};
	//create VBO and VAO objects
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//unbind buffer, not necessarily
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);







	//render loop
	while (!glfwWindowShouldClose(mywindow))
	{
		//process input
		processInput(mywindow);
		//render
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//
		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		//update uniform
		double varying_time = glfwGetTime();
		float green = static_cast<float>((sin(varying_time) + 1.0f) / 2.0f);
		int uniform_location = glGetUniformLocation(shader_program, "breath");
		glUniform4f(uniform_location, 0.0f, green, 0.0f, 1.0f);
		//draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		

		glfwSwapBuffers(mywindow);
		glfwPollEvents();
	}

	//释放资源
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window,int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}