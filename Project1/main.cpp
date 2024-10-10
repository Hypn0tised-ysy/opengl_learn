#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "shader.h"
#include "Camera.h"
#include "stb_image.h"
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
//screen settings
unsigned int constexpr Width = 1024;
unsigned int constexpr Height = 768;
//camera
Camera camera;
bool firstMouse = true;//鼠标第一次进入窗口，用这个变量防止鼠标进入窗口时camera乱飞
float lastX = Width / 2;
float lastY = Height / 2;
//light
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightColor=glm::vec3(1.0f);
//mouse input settings
float last_frame=0.0f;
float delta_time=0.0f;
//phong coefficient
float AMBIENT = 0.1f;
float SPECULAR = 0.5f;
void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
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
	//mouse input
	glfwSetInputMode(mywindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(mywindow, mouse_callback);
	glfwSetScrollCallback(mywindow, scroll_callback);
	//initialize glad
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout<<"fail to initialize glad\n";
	}


	//depth test
	glEnable(GL_DEPTH_TEST);
	//vertex data
	float vertices[] = {
		//position            //normal
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	//create VBO and VAO objects
	unsigned int VBO, VAO,lightVAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1,&VAO);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	//
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind buffer, not necessarily
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//activate shader
	Shader myshader("./phongVertex.vs", "./phongFragment.fs");//进行光照计算的shader
	//shader for light
	Shader lightShader("./lightVertex.vs", "./lightFragment.fs");//渲染光源的shader
	//render loop
	while (!glfwWindowShouldClose(mywindow))
	{
		//process input
		processInput(mywindow);

		//render
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		lightPos.x = sin(glfwGetTime()/2.0) * 3.0f;
		lightPos.z = cos(glfwGetTime()/2.0) * 3.0f;
		//
		myshader.use();

		//transforming matrixes
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection= glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)Width / (float)Height, 0.1f, 100.0f);
		myshader.setMatrix("view", view);
		myshader.setMatrix("projection", projection);
		glm::mat4 model = glm::mat4(1.0f);
		myshader.setMatrix("model", model);
		//
 		myshader.setVec3("lightColor",lightColor);
		myshader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		myshader.setVec3("lightPos", lightPos);
		myshader.setVec3("eyePos", camera.Position);
		myshader.setFloat("ambient", AMBIENT);
		myshader.setFloat("specular", SPECULAR);
		//
		glBindVertexArray(VAO);
		//draw
		glDrawArrays(GL_TRIANGLES, 0,36);
		// also draw the lamp object
		lightShader.use();
		lightShader.setMatrix("projection", projection);
		lightShader.setMatrix("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lightShader.setMatrix("model", model);
		lightShader.setVec3("lightColor", lightColor);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(mywindow);
		glfwPollEvents();//poll events表示处理事件，在这个函数中会调用各种call back函数响应诸如移动鼠标等事件
	}

	//释放资源
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();
	
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window,int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{

	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	//wasd input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.PorcessKeyboard(Camera_Movement::FORWARD, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.PorcessKeyboard(Camera_Movement::LEFT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.PorcessKeyboard(Camera_Movement::BACKWARD, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.PorcessKeyboard(Camera_Movement::RIGHT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camera.PorcessKeyboard(Camera_Movement::DOWN, delta_time);
	}
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camera.PorcessKeyboard(Camera_Movement::UP, delta_time);
	}



		
}
void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float xoffset = xPos-lastX;
	float yoffset = -(yPos-lastY);//pitch增大实际上是look down
	lastX = xPos;
	lastY = yPos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}