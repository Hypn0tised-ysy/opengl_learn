#pragma once
#include <glad/glad.h>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
float constexpr ZOOM = 45.0f;//field of view
float constexpr YAW = -90.0f;
float constexpr PITCH = 0.0f;
float constexpr SPEED = 2.5f;//视角移动速度，wasd控制
float constexpr SENSITIVITY = 0.1f;//鼠标灵敏度

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};
class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Up;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	//Euler angle
	float Yaw;
	float Pitch;
	//camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	//constructor
	//构造函数可以指定camera的positon和up向量
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	//get view matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	//keyboard input
	void PorcessKeyboard(Camera_Movement direction, float delta_time)
	{
		float velocity = MovementSpeed * delta_time;
		glm::vec3 front = glm::normalize(glm::vec3(Front.x,0.0f, Front.z));
		glm::vec3 left = glm::cross(WorldUp, front);
		switch (direction)
		{
		case FORWARD:
			Position += front * velocity;
			break;
		case BACKWARD:
			Position -= front* velocity;
			break;
		case LEFT:
			Position += left * velocity;
			break;
		case RIGHT:
			Position -= left * velocity;
			break;
		default:
			break;
		}
	}
	//mouse input
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;
		Yaw += xoffset;
		Pitch+= yoffset;
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}
		updateCameraVectors();
	}
	void ProcessMouseScroll(float yoffset)
	{
		//改变fov来实现zoom-in\out 的效果
		Zoom -= (float)yoffset;
		if (Zoom < 1.0f)
			Zoom = 1.0f;
		if (Zoom > 45.0f)
			Zoom = 45.0f;
	}
private:
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)); 
		front.y = sin(glm::radians(Pitch)); 
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)); 
		Front = glm::normalize(front);
		//
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

