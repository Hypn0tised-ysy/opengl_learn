#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"
unsigned int constexpr Width = 1024;
unsigned int constexpr Height = 768;
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow* window);
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



	//vertex data
	float vertex[] = {
		//location       //color        //texture
		-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,//bottom left
		-0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,//top left
		0.5f,-0.5f,0.0f,0.0f,0.0f,1.0f,1.0f,0.0f,//bottom right
		0.5f,0.5f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f//top right
	};
	unsigned int indices[] = {
		0,1,3,
		0,2,3
	};
	//create VBO and VAO objects
	unsigned int VBO, VAO,EBO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	//load texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//set wrapping parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load image, create texture and generate mipmaps
	int width, height, nrChannels;
	unsigned char* data = stbi_load("C:/Users/14814/Desktop/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);


	//unbind buffer, not necessarily
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//activate shader
	Shader myshader("./vertex.vs", "./fragment.fs");
	myshader.use();
	int offset_location = glGetUniformLocation(myshader.ID, "horizontal_offset");
	glUniform1f(offset_location, 0.3f);

	//render loop
	while (!glfwWindowShouldClose(mywindow))
	{
		//process input
		processInput(mywindow);
		//render
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//
		glBindVertexArray(VAO);
		//update uniform
		double varying_time = glfwGetTime();
		float green = static_cast<float>((sin(varying_time) + 1.0f) / 2.0f);
		//draw
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		glfwSwapBuffers(mywindow);
		glfwPollEvents();
	}

	//释放资源
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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