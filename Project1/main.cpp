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
"void main()\n"
"{\n"
"gl_Position=vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
"}\0";
//fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 breath;\n"
"void main()\n"
"{\n"
"FragColor=breath;\n"
"}\n\0";
//another fragment shader
const char* fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 breath;\n"
"void main()\n"
"{\n"
"FragColor=breath;\n"
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
	glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
	glCompileShader(fragmentShader);
	glCompileShader(fragmentShader1);
	//check if the shader is compiled successfully
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, error_info);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << error_info << "\n";
	}
	glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader1, 512, NULL, error_info);
		std::cout << "FRAGMENT SHADER COMPILATION FAILED\n" << error_info << "\n";
	}
	//create shader program
	unsigned int shader_program = glCreateProgram(),shader_program1=glCreateProgram();
	//first shader program
	glAttachShader(shader_program,vertexShader);
	glAttachShader(shader_program,fragmentShader);
	//second shader program
	glAttachShader(shader_program1, vertexShader);
	glAttachShader(shader_program1, fragmentShader1);
	//
	glLinkProgram(shader_program);
	glLinkProgram(shader_program1);
	//check link status
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, error_info);
		std::cout << "SHADER PROGRAM LINKING FAILED\n"<<error_info<<"\n";
	}
	glGetProgramiv(shader_program1, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program1, 512, NULL, error_info);
		std::cout << "SHADER PROGRAM LINKING FAILED\n" << error_info << "\n";
	}
	//delete shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader1);
	//vertex data
	float vertex[] = {
		-0.5f,-0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f,
	};
	float vertex1[] = { 
		0.5f,-0.5f,0.0f,
		0.0f,0.5f,0.0f,
		1.0f,1.0f,0.5f 
	};
	//create VBO and VAO objects
	unsigned int VBO, VAO,VBO1,VAO1;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//unbind buffer, not necessarily
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//another set of vao and vbo
	glGenBuffers(1, &VBO1);
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex1), vertex1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//unbind buffer, not necessarily
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);







	//render loop
	while (!glfwWindowShouldClose(mywindow))
	{
		//process input
		processInput(mywindow);
		//render
		glClearColor(0.45f, 0.32f, 0.15f, 1.0f);
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
		//another shader
		//draw another triangle with another set of vao and vbo
		glUseProgram(shader_program1);
		glBindVertexArray(VAO1);
		//update uniform
		double varying_time_blue = glfwGetTime();
		float blue = static_cast<float>((sin(varying_time_blue) + 1.0f) / 2.0f);
		int uniform_location_blue = glGetUniformLocation(shader_program, "breath");
		glUniform4f(uniform_location_blue, 0.0f, 0.0f, blue, 1.0f);
		//draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);
		//
		

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