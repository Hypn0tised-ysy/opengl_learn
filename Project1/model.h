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
class Model//�洢model�ļ�������mesh��Ϣ������
{
	public:
		std::vector<Mesh> meshes;
		std::vector<Texture> textures_loaded;//�Ѿ����ع�����ͼ
		std::string directory; //���ص�ģ���ļ����ڵ�Ŀ¼,����ٶ�texture���ŵ�model�ļ���ͬ��Ŀ¼�����ڼ���model��Ӧ�������ļ�
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

