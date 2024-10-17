#pragma once
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <string>
#include <vector>
#define MAX_BONE_INFLUENCE 4
struct Vertex
{
	//position
	glm::vec3 Postion;
	//normal
	glm::vec3 Normal;
	//texCoord
	glm::vec2 TexCoords;
	//正切空间相关
	//tangent
	glm::vec3 Tangent;
	//骨骼相关
	//bitangent
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};
struct Texture
{
	unsigned int id;
	std::string type;//纹理类型，specular、diffuse之类
	std::string path;//纹理路径
};
class Mesh
{
public:
	//绘制mesh需要的数据
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int VAO;
	//constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	//
	void Draw(Shader& shader);
private:
	unsigned int VBO, EBO;
	void setupMesh();
};

