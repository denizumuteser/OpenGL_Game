#include"Model.h"
#include <string>
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
//std::vector<Mesh> Model::meshes;
//std::vector<Texture> Model::textures_loaded;

// constructor, expects a filepath to a 3D model.
Model::Model(
    const std::string& path,
    unsigned int instancing,
    std::vector<glm::mat4> instanceMatrix)
{
    Model::instancing = instancing;
    Model::instanceMatrix = instanceMatrix;
    //gammaCorrection = gamma;
    loadModel(path);
    //std::cout << "number of meshes from model : " << meshes.size() << std::endl;
}

// draws the model, and thus all its meshes
void Model::Draw(
    Shader& shader,
    Camera& camera,
    glm::vec3 translation,
    glm::quat rotation,
    glm::vec3 scale)
{

    // Go over all meshes and draw each one
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader, camera, glm::mat4(1.0f), translation, rotation, scale);
    } 
}

// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(std::string const& path)
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    this->processNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
        //meshes.push_back(Mesh(vertices, indices, textures, instancing, instanceMatrix));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        //color TO DELETE
        //glm::vec3 vector2 = glm::vec3(1.0f);
        //vertex.color = vector2;
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texUV = vec;
            // tangent
            //vector.x = mesh->mTangents[i].x;
            //vector.y = mesh->mTangents[i].y;
            //vector.z = mesh->mTangents[i].z;
            //Vertex.Tangent = vector;
            // bitangent
            //vector.x = mesh->mBitangents[i].x;
            //vector.y = mesh->mBitangents[i].y;
            //vector.z = mesh->mBitangents[i].z;
            //Vertex.Bitangent = vector;
        }
        else
        {
            vertex.texUV = glm::vec2(0.0f, 0.0f);
        }
            

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    //std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
    //textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    //std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "height");
    //textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    //std::cout << textures.size() << std::endl;
    //return Mesh(vertices, indices, textures);
    return Mesh(vertices, indices, textures, instancing, instanceMatrix);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    //std::cout << (mat->GetTextureCount(type)) << std::endl;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {

            if (std::strcmp(textures_loaded[j].path, str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if (!skip)

            
        {   // if texture hasn't been loaded already, load it
            //Texture texture = Texture(str.C_Str(), typeName.c_str(), 0, GL_RGB, GL_UNSIGNED_BYTE);
            Texture texture;
            texture.ID = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName.c_str();
            texture.path = str.C_Str();

            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}

void Model::updateCollisionBox(float addedx, float addedy, float addedz)
{
    minX += addedx;
    maxX += addedx;
    minY += addedy;
    maxY += addedy;
    minZ += addedz;
    maxZ += addedz;
}

bool Model::checkCollision(Model otherModel)
{
    return (
        minX <= otherModel.maxX &&
        maxX >= otherModel.minX &&
        minY <= otherModel.maxY &&
        maxY >= otherModel.minY &&
        minZ <= otherModel.maxZ &&
        maxZ >= otherModel.minZ
        );
}

void Model::setCollisionBox(float minx, float maxx, float miny, float maxy, float minz, float maxz)
{
    minX = minx;
    maxX = maxx;
    minY = miny;
    maxY = maxy;
    minZ = minz;
    maxZ = maxz;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    std::cout << filename << std::endl;
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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::setCollisionBox()
{
    minX = position.x - 0.05f;
    maxX = position.x + 0.05f;
    minY = position.y - 0.1f;
    maxY = position.y + 0.1f;
    minZ = position.z - 0.05f;
    maxZ = position.z + 0.05f;
}

void Model::move(glm::vec3 directionVec)
{
    position.x += directionVec.x * speed * 0.0005;
    position.y += directionVec.y * speed * 0.0005;
    position.z += directionVec.z * speed * 0.0005;

    updateCollisionBox((directionVec.x * speed * 0.0005), (directionVec.y * speed * 0.0005), (directionVec.z * speed * 0.0005));
}

bool Model::checkCollision(float BminX, float BmaxX, float BminY, float BmaxY, float BminZ, float BmaxZ)
{
    return (
        minX <= BmaxX &&
        maxX >= BminX &&
        minY <= BmaxY &&
        maxY >= BminY &&
        minZ <= BmaxZ &&
        maxZ >= BminZ
        );
}
