
#include "shader.h"
Shader::Shader(char const* vertexShaderPath, char const* fragmentShaderPath)
{
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;
	std::string vertexCode, fragmentCode;
	char const* vertexShaderCode, *fragmentShaderCode;
	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//open files
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);
		//read content
		std::stringstream vertexShaderStream, fragmentShaderStream;
		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		//close files
		vertexShaderFile.close();
		fragmentShaderFile.close();
		//load string
		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();
	}
	catch(std::ifstream::failure& error)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << error.what()<<"\n";
	}
	vertexShaderCode = vertexCode.c_str();
	fragmentShaderCode = fragmentCode.c_str();
	unsigned int vertex, fragment;
	//vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileError(vertex, "VERTEX");
	//fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileError(fragment, "FRAGMENT");
	//link
	ID= glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkCompileError(ID, "PROGRAM");
	//delete shader
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}
void Shader::use()
{
	glUseProgram(ID);
}
void Shader::setFloat(std::string const& name, float value)const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setInt(std::string const& name, int value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setMatrix(std::string const& name, glm::mat4& matrix)
{
	unsigned int transformLoc = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setVec3(std::string const& name, glm::vec3 const& vec)
{
	unsigned int Vec3Loc = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(Vec3Loc, 1, &vec[0]);
}
void Shader::setVec3(std::string const& name, float x, float y, float z)
{
	unsigned int Vec3Loc = glGetUniformLocation(ID, name.c_str());
	glUniform3f(Vec3Loc, x, y, z);
}
void Shader::checkCompileError(unsigned int shader, std::string const& type)//type要么是检查shader编译是否出错，要么检查shader program链接是否出错
{
	int success;
	size_t const info_size = 1024;
	char errorInfo[info_size];
	if (type != "PROGRAM")//检查shader 编译是否出错
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, info_size, NULL, errorInfo);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type " << type << "\n" << errorInfo << "\n-------------------------\n";
		}
	}
	else//检查 shader program链接是否出错
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, info_size, NULL, errorInfo);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type " << type << "\n" << errorInfo << "\n-------------------------\n";
		}
	}
}