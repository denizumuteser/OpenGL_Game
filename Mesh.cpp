#include "Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO(vertices);
	// Generates Element Buffer Object and links it to indices
	EBO EBO(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); //cordinate
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); //normal
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); //color
	VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float))); //textUV
	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}


void Mesh::Draw(Shader& shader, Camera& camera)
{
	// Bind shader to be able to access uniforms
	shader.Activate();
	VAO.Bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 1;
	unsigned int numSpecular = 1;
	//std::cout << textures.size() << std::endl;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string num;
		std::string type = textures[i].type;
		if (type == "texture_diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "texture_specular")
		{
			num = std::to_string(numSpecular++);
		}
		shader.Activate();
		//glUniform1i(glGetUniformLocation(shader.ID, (type + num).c_str()), i);
		
		// Gets the location of the uniform
		//GLuint texUni = glGetUniformLocation(shader.ID, uniform);
		// Shader needs to be activated before changing the value of a uniform
		//shader.Activate();
		// Sets the value of the uniform
		//glUniform1i(texUni, unit);

		//glActiveTexture(GL_TEXTURE0 + textures[i].unit);
		//glBindTexture(GL_TEXTURE_2D, textures[i].ID);
		//std::cout << (type + num).c_str()  << std::endl;
		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	camera.Matrix(shader, "camMatrix");

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	
}