#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm-master/glm/glm.hpp>
#include <glm-master/glm/gtc/matrix_transform.hpp>
#include <glm-master/glm/gtc/type_ptr.hpp>
#include "shader.h">
#include <vector>
#include "mesh.h"
class Model//存储model文件的所有mesh信息并绘制
{
	public:
		std::vector<Mesh> meshes;
		std::vector<Texture> textures_loaded;//已经加载过的贴图
		std::string directory; //加载的模型文件所在的目录,这里假定texture会存放到model文件的同根目录，用于加载model对应的纹理文件
		bool gammaCorrection;
		Model(const char* path, bool gamma = false) : gammaCorrection(gamma)
		{
			loadModel(path);
		}
		void Draw(Shader& shader)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(shader);
		}
private:
	void loadModel(std::string path);
	void processNode(aiNode* node, aiScene const* scene);
	Mesh processMesh(aiMesh* mesh, aiScene const* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typename);
};

