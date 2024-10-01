#pragma once
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <iostream>
#include <string>
class Shader//实际对应的是shader program的概念
{
public:
	unsigned int ID;
	Shader(char const* vertexShaderPath, char const* fragmentShaderPath);
	void use();
	void setFloat(std::string const& name, float value)const;
	void setInt(std::string const& name, int value)const;
private:
	void checkCompileError(unsigned int shader, std::string const& type);

};

