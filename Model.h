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
	Model(const std::string& file, unsigned int instancing = 1, std::vector<glm::mat4> instanceMatrix = {});

	void Draw
	(
		Shader& shader,
		Camera& camera,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);

	void loadModel(std::string const& path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);
	std::string directory;

	// All the meshes and transformations
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;
	std::vector<glm::mat4> instanceMatrix;

	float speed = 0;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	Shader shader;
	glm::vec3 moveDirection;
	glm::quat rotation;

	void updateCollisionBox(float x, float y, float z);
	void setCollisionBox();
	void setCollisionBox(float minx, float maxx, float miny, float maxy, float minz, float maxz);
	bool checkCollision(Model otherModel);
	bool checkCollision(float BminX, float BmaxX, float BminY, float BmaxY, float BminZ, float BmaxZ);

	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	void move(glm::vec3 directionVec);

private:
	// model data 
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

	unsigned int instancing;

	// All the meshes and transformations

	//static bool gammaCorrection;

	// Variables for easy access
	const char* file;

	
	

	// Loads a single mesh by its index
	void loadMesh(unsigned int indMesh);
};
#endif