#pragma once
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
class Shader//实际对应的是shader program的概念
{
public:
	unsigned int ID;
	Shader(char const* vertexShaderPath, char const* fragmentShaderPath);
	void use();
	void setFloat(std::string const& name, float value)const;
	void setInt(std::string const& name, int value)const;
	void setMatrix(std::string const& name, glm::mat4& matrix);
	void setVec3(std::string const& name, glm::vec3 const& vec);
	void setVec3(std::string const& name, float x,float y,float z);
private:
	void checkCompileError(unsigned int shader, std::string const& type);

};

