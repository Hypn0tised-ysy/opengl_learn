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
	//���пռ����
	//tangent
	glm::vec3 Tangent;
	//�������
	//bitangent
	glm::vec3 Bitangent;
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	float m_Weights[MAX_BONE_INFLUENCE];
};
struct Texture
{
	unsigned int id;
	std::string type;//�������ͣ�specular��diffuse֮��
	std::string path;//����·��
};
class Mesh
{
public:
	//����mesh��Ҫ������
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

