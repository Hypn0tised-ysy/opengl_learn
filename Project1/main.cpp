#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
unsigned int constexpr Width = 1024;
unsigned int constexpr Height = 768;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);//cameraFront定义了camera注视的点相对于相机位置的方向和距离，注视中心center=cameraPos+cameraFront
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float last_frame=0.0f;
float delta_time=0.0f;
float mixValue = 0.0f;
float preMixValue = -1.0f;
bool pressed = false;//用来按键防抖的天才设计
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


	//depth test
	glEnable(GL_DEPTH_TEST);
	//vertex data
	float vertices[] = {
		//location            //texture
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// world space positions of our cubes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//create VBO and VAO objects
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//load texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//set wrapping parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load image, create texture and generate mipmaps
	stbi_set_flip_vertically_on_load(true);//竖直翻转图像，因为openGL的y轴零点在下方，很多images y轴零点在上方
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
	//another texture
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	//set wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//set filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load image, create texture and generate mipmaps
	data = stbi_load("C:/Users/14814/Desktop/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
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
	myshader.setInt("texture1", 0);
	myshader.setInt("texture2", 1);
	//render loop
	while (!glfwWindowShouldClose(mywindow))
	{
		//process input
		processInput(mywindow);
		myshader.setFloat("mixValue", mixValue);
		//render
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//transforming matrixes
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection= glm::mat4(1.0f);
		//define a camera
		view = glm::lookAt(cameraPos,cameraPos+cameraFront,cameraUp);
		projection = glm::perspective(glm::radians(45.0f), (float)Width / (float)Height, 0.1f, 100.0f);
		myshader.setMatrix("view", view);
		myshader.setMatrix("projection", projection);
		//
		glBindVertexArray(VAO);
		for (int i = 0; i < 10; ++i)
		{
			float rotate_angle = 20.0f * i;
			glm::mat4 model = glm::mat4(1.0f);;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(rotate_angle), glm::vec3(1.0f, 0.3f, 0.5f));
			if (i % 3 == 0)
			{
				model = glm::rotate(model, glm::radians(float(10*glfwGetTime())), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			myshader.setMatrix("model",model );
			//draw
			glDrawArrays(GL_TRIANGLES, 0,36);
		}



		

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
	float constexpr camera_speed = 2.0f;
	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (!pressed)//如果上一帧上下键被按过，则直接跳过这一帧的上下方向键的判定
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			mixValue += 0.1f;
			if (mixValue > 1.0f)
				mixValue = 1.0f;
			pressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			mixValue -= 0.1f;
			if (mixValue < 0.0f)
				mixValue = 0.0f;
			pressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraPos += cameraFront * camera_speed * delta_time;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraPos += glm::normalize(glm::cross(cameraUp,cameraFront)) * camera_speed * delta_time;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraPos -= cameraFront * camera_speed*delta_time;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraPos -= glm::normalize(glm::cross(cameraUp, cameraFront)) * camera_speed * delta_time;
		}
		

	}
	else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS )//当上一帧按下了上下方向键，且这一帧上下方向键没被按下，则把pressed设置回false
	{
		pressed = false;
	}
    //用来按键防抖
	if(preMixValue!=mixValue)
	std::cout << mixValue << "\n";
	preMixValue = mixValue;
		
}