//opengl
#include <glad/glad.h>
#include <glfw3.h>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
//imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//stl
#include <iostream>
//
#include "shader.h"
#include "Camera.h"
#include "stb_image.h"
#include "model.h"
#include "mesh.h"
//screen settings
unsigned int constexpr SideBarWidth = 300;
unsigned int constexpr Width = 1024 + SideBarWidth;
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
//imgui 
bool showMenu = false;
float backgroundIntensity = 0.2f; 
float rotateSpeed = 1.0f;
bool pressed = false;//按键防抖
float intensity = 1.0f;//点光源光强
//cube rotation
//游戏时间
float t;
float last_t;
float game_time;//游戏时间，在按下esc键后暂停
float pause_time=0.0f;//暂停时的时间
float pass_time = 0.0f;
float last_pass_time = 0.0f;
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
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(mywindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();


	//depth test
	glEnable(GL_DEPTH_TEST);
	/*vertex data*/
		//vertex data
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	// positions all containers
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
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	/*vao,vbo stuff*/
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//unbind buffer, not necessarily
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//load texture
	unsigned int diffuseMap = loadTexture("C:/Users/14814/Desktop/resources/pictures/rendering/wall_my_info.png");
	unsigned int specularMap = loadTexture("C:/Users/14814/Desktop/quickaccess/container2_specular.png");
	unsigned int wallinfo= loadTexture("C:/Users/14814/Desktop/resources/pictures/rendering/wall_my_info.png");
	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	// bind wall_info
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, wallinfo);
	//unbind buffer, not necessarily
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//shader for boxes
	Shader myshader("./shader/multiLight.vs", "./shader/multiLight.fs");
	//shader for light
	Shader lightShader("./shader/lightVertex.vs", "./shader/lightFragment.fs");//渲染光源的shader
	//render loop
	while (!glfwWindowShouldClose(mywindow))
	{

        // Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		float current_time = glfwGetTime();
		float dt=0.0f;
		if(!showMenu)
		{
			last_pass_time = pass_time;
			pass_time = current_time - pause_time;//时间差为打开menu界面的时间
			dt = pass_time - last_pass_time;//菜单未唤出时，相邻两帧经过的时间
			game_time += rotateSpeed*dt;
		}
		else//唤出菜单
		{
			pause_time = glfwGetTime();
			pass_time = 0.0f;
		}


		t +=50*rotateSpeed*dt  ;//控制cube旋转的时间变量，只有菜单不显示时才更新,游戏时间该怎么实现呢=-=
		//process input
		processInput(mywindow);
		// 显示侧边栏菜单
		if (showMenu)
		{
			ImGui::SetNextWindowPos(ImVec2(Width - SideBarWidth, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(SideBarWidth, Height), ImGuiCond_Always);
			ImGui::Begin("Pause Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

			ImGui::Text("Game Paused");
			if (ImGui::Button("Continue", ImVec2(120, 0)))
			{
				showMenu = false;
				glfwSetInputMode(mywindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 恢复鼠标控制
			}
			if (ImGui::Button("Exit", ImVec2(120, 0)))
			{
				glfwSetWindowShouldClose(mywindow, true);
			}

			ImGui::SliderFloat("Light Intensity", &intensity, 0.0f, 1.0f);
			ImGui::SliderFloat("Background Intensity", &backgroundIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("Rotating Speed", &rotateSpeed, 0.0f, 1.0f);
			ImGui::End();
		}
		else
		{
			// 常规控制面板
			ImGui::SetNextWindowPos(ImVec2(Width - SideBarWidth, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(SideBarWidth, Height), ImGuiCond_Always);
			ImGui::Begin("Control Panel", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

			ImGui::Text("Light Controls:");
			ImGui::SliderFloat("Light Intensity", &intensity, 0.0f, 1.0f);
			ImGui::SliderFloat("Background Intensity", &backgroundIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("Rotating Speed", &rotateSpeed, 0.0f, 1.0f);

			ImGui::End();
		}


		//render
		glClearColor(backgroundIntensity, backgroundIntensity, backgroundIntensity, 1.0f);
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
		myshader.setInt("material.wallinfo", 2);
		//material
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
		myshader.setFloat("pointLight[0].intensity", intensity);
		myshader.setVec3("pointLight[0].ambient", lightAmbient);
		myshader.setVec3("pointLight[0].diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("pointLight[0].specular", lightSpecular);
		myshader.setVec3("pointLight[0].lightPos", pointLightPositions[0]);
		//attenuation
		myshader.setFloat("pointLight[0].constant", constant);
		myshader.setFloat("pointLight[0].linear", linear);
		myshader.setFloat("pointLight[0].quadratic", quadratic);
		//point light 2
		myshader.setFloat("pointLight[1].intensity", intensity);
		myshader.setVec3("pointLight[1].ambient", lightAmbient);
		myshader.setVec3("pointLight[1].diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("pointLight[1].specular", lightSpecular);
		myshader.setVec3("pointLight[1].lightPos", pointLightPositions[1]);
		//attenuation
		myshader.setFloat("pointLight[1].constant", constant);
		myshader.setFloat("pointLight[1].linear", linear);
		myshader.setFloat("pointLight[1].quadratic", quadratic);
		//point light 3
		myshader.setFloat("pointLight[2].intensity", intensity);
		myshader.setVec3("pointLight[2].ambient", lightAmbient);
		myshader.setVec3("pointLight[2].diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("pointLight[2].specular", lightSpecular);
		myshader.setVec3("pointLight[2].lightPos", pointLightPositions[2]);
		//attenuation
		myshader.setFloat("pointLight[2].constant", constant);
		myshader.setFloat("pointLight[2].linear", linear);
		myshader.setFloat("pointLight[2].quadratic", quadratic);
		//point light 4
		myshader.setFloat("pointLight[3].intensity", intensity);
		myshader.setVec3("pointLight[3].ambient", lightAmbient);
		myshader.setVec3("pointLight[3].diffuse", lightDiffuse); // darken diffuse light a bit
		myshader.setVec3("pointLight[3].specular", lightSpecular);
		myshader.setVec3("pointLight[3].lightPos", pointLightPositions[3]);
		//attenuation
		myshader.setFloat("pointLight[3].constant", constant);
		myshader.setFloat("pointLight[3].linear", linear);
		myshader.setFloat("pointLight[3].quadratic", quadratic);
		//

		myshader.use();
		//cube
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{ // calculate the model matrix for each object and pass it to shader before drawing 
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i % 3 == 0)
				model = glm::scale(model, glm::vec3(1.5f));
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			if (i % 3 == 0)
				model = glm::rotate(model, glm::radians(t), glm::vec3(1.0f, 0.3f, 0.5f));
			myshader.setMatrix("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//
		glfwSwapBuffers(mywindow);
		glfwPollEvents();//poll events表示处理事件，在这个函数中会调用各种call back函数响应诸如移动鼠标等事件
	}

	//释放资源
	glfwTerminate();
	//
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	//
	return 0;
}
void framebuffer_size_callback(GLFWwindow* window,int width, int height)
{
	glViewport(0, 0, width-SideBarWidth, height);
}
void processInput(GLFWwindow* window)
{

	float current_frame = glfwGetTime();
	delta_time = current_frame - last_frame;
	last_frame = current_frame;
	// 切换菜单显示状态
	if (!pressed)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			pressed = true;
			showMenu = !showMenu;
			firstMouse = true;//防止鼠标乱飞
			// 切换鼠标输入模式
			if (showMenu) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
		pressed = false;
	// 如果菜单显示，不处理摄像机移动输入
	if (showMenu) return;

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
	if (!showMenu)
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}
		float xoffset = xPos - lastX;
		float yoffset = -(yPos - lastY);//pitch增大实际上是look down
		lastX = xPos;
		lastY = yPos;
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if(!showMenu)
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