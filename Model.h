#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include"Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
class Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const std::string& file);

	void Draw(Shader& shader, Camera& camera);
	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);
	std::string directory;
private:
	// model data 
	static std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	
	//static bool gammaCorrection;

	// Variables for easy access
	const char* file;

	// All the meshes and transformations
	std::vector<Mesh> meshes;

};
#endif