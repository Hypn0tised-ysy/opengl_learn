#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "shader.h"
#include "Camera.h"
#include "stb_image.h"
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
#include "model.h"
#include "mesh.h"
//screen settings
unsigned int constexpr Width = 1024;
unsigned int constexpr Height = 768;
//camera
Camera camera;
bool firstMouse = true;//鼠标第一次进入窗口，用这个变量防止鼠标进入窗口时camera乱飞
float lastX = Width / 2;
float lastY = Height / 2;
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
unsigned int loadTexture(char const* path);
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

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	Model backpack("C:/Users/14814/Desktop/resources/art/nanosuit/nanosuit.obj");//model会和model矩阵重名，尴尬

	//activate shader
	Shader myshader("./model.vs", "./model.fs");//进行光照计算的shader
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
		glm::vec3 lightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 lightAmbient = glm::vec3(0.2f, 0.2f, 0.2f);//太暗了我受不了=-=
		glm::vec3 lightDir = glm::vec3(-0.2f, -1.0f, -0.3f);
		//
		myshader.setInt("material.diffuse", 0);
		myshader.setInt("material.specular", 1);
		//material
		myshader.setVec3("material.ambient", 0.4f, 0.4f, 0.8f);
		myshader.setVec3("material.diffuse", 0.4f, 0.4f, 0.8f);
		myshader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		myshader.setFloat("material.shininess", 32.0f);
		//eye
		myshader.setVec3("eyePos", camera.Position);
		//flash light
		myshader.setVec3("flashLight.ambient", lightAmbient);
		myshader.setVec3("flashLight.diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("flashLight.specular", lightSpecular);
		//attenuation
		myshader.setFloat("flashLight.constant", 1.0f); 
		myshader.setFloat("flashLight.linear", 0.09f); 
		myshader.setFloat("flashLight.quadratic", 0.032f);
		//
		float cutOff = glm::cos(glm::radians(12.5f));
		float outerCutOff = glm::cos(glm::radians(17.0f));
		myshader.setVec3("flashLight.lightPos", camera.Position);
		myshader.setVec3("flashLight.spotDir", camera.Front);
		myshader.setFloat("flashLight.cutOff", cutOff);
		myshader.setFloat("flashLight.outerCutOff", outerCutOff);
		//directional light
		myshader.setVec3("dirLight.ambient", lightAmbient);
		myshader.setVec3("dirLight.diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("dirLight.specular", lightSpecular);
		myshader.setVec3("dirLight.direction", lightDir);
		
		float constant = 1.0f, linear = 0.09f, quadratic = 0.032f;
		//point light 1
		myshader.setVec3("pointLight[0].ambient", lightAmbient);
		myshader.setVec3("pointLight[0].diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("pointLight[0].specular", lightSpecular);
		myshader.setVec3("pointLight[0].lightPos", pointLightPositions[0]);
		//attenuation
		myshader.setFloat("pointLight[0].constant", constant);
		myshader.setFloat("pointLight[0].linear", linear);
		myshader.setFloat("pointLight[0].quadratic", quadratic);
		//point light 2
		myshader.setVec3("pointLight[1].ambient", lightAmbient);
		myshader.setVec3("pointLight[1].diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("pointLight[1].specular", lightSpecular);
		myshader.setVec3("pointLight[1].lightPos", pointLightPositions[1]);
		//attenuation
		myshader.setFloat("pointLight[1].constant", constant);
		myshader.setFloat("pointLight[1].linear", linear);
		myshader.setFloat("pointLight[1].quadratic", quadratic);
		//point light 3
		myshader.setVec3("pointLight[2].ambient", lightAmbient);
		myshader.setVec3("pointLight[2].diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("pointLight[2].specular", lightSpecular);
		myshader.setVec3("pointLight[2].lightPos", pointLightPositions[2]);
		//attenuation
		myshader.setFloat("pointLight[2].constant", constant);
		myshader.setFloat("pointLight[2].linear", linear);
		myshader.setFloat("pointLight[2].quadratic", quadratic);
		//point light 4
		myshader.setVec3("pointLight[3].ambient", lightAmbient);
		myshader.setVec3("pointLight[3].diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("pointLight[3].specular", lightSpecular);
		myshader.setVec3("pointLight[3].lightPos", pointLightPositions[3]);
		//attenuation
		myshader.setFloat("pointLight[3].constant", constant);
		myshader.setFloat("pointLight[3].linear", linear);
		myshader.setFloat("pointLight[3].quadratic", quadratic);
		//
		backpack.Draw(myshader);
		//draw

		// also draw the lamp object
		//lightShader.use();
		//lightShader.setMatrix("projection", projection);
		//lightShader.setMatrix("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		//lightShader.setMatrix("model", model);
		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);


		glfwSwapBuffers(mywindow);
		glfwPollEvents();//poll events表示处理事件，在这个函数中会调用各种call back函数响应诸如移动鼠标等事件
	}

	//释放资源
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
// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}