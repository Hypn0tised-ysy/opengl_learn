#include "model.h"
#include "stb_image.h"
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	aiScene const* scene=importer.ReadFile(path,aiProcess_Triangulate|aiProcess_FlipUVs);//第二个参数对模型三角形划分，翻转uv坐标，模型信息被读取到assimp的通用结构体scene中
	//
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout<<"ERROR::ASSIMP::"<<importer.GetErrorString()<<std::endl;
		return;
	}
	//
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}
void Model::processNode(aiNode* node, aiScene const* scene)
{
	//process all the node'smeshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);//递归调用，从而把scen中的所有meshes都加入到model中
	}
}
Mesh Model::processMesh(aiMesh* mesh,aiScene const* scene)//把assimp的mesh读取到程序自定义的mesh中
{
	//mesh数据
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	//
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//retrieve data
		//position
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;//mVertices存储顶点的位置向量，什么鬼变量命名
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Postion = vector;
		//normal
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		//texture
		if (mesh->mTextureCoords[0]) // 检查mesh是否包含texture数据
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
			// tangent
			//vector.x = mesh->mTangents[i].x;
			//vector.y = mesh->mTangents[i].y;
			//vector.z = mesh->mTangents[i].z;
			//vertex.Tangent = vector;
			// bitangent
			//vector.x = mesh->mBitangents[i].x;
			//vector.y = mesh->mBitangents[i].y;
			//vector.z = mesh->mBitangents[i].z;
			//vertex.Bitangent = vector;
		}
		else
		vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		//
		vertices.push_back(vertex);
	}
	//process indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);//个人感觉indices声明成vector<veindicesctor<int>>更好理解，把单个face对应的下标作为单个元素存进
	}
	//process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];//把material中的所有texture读取出来
		//loadMaterialTexures会把指定类型的texture全部读取，并以vector<Texture>的形式返回
		// 1. diffuse maps
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());//从texture的末尾处插入diffuseMaps首到尾的元素（就是所有元素）
		// 2. specular maps
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		//遍历已经加载过的texture，如果和某一texture路径名相等，说明是同一张贴图，则将skip设置为真
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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
