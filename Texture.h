#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;
	const char* path;

	Texture();

	Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

	Texture(const char* image, GLuint slot, GLenum format, GLenum pixelType);

	unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

	// Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binds a texture
	void Bind();
	// Unbinds a texture
	void Unbind();
	// Deletes a texture
	void Delete();
};
#endif