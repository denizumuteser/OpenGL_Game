#include "main.h"

int main()
{
	//init window
	GLFWwindow* window = initilizeWindow();
	// Error check if the window fails to create
	if (window == NULL)
	{
		return -1;
	}

	// Texture data
	Texture textures[]
	{
		//Texture("planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		//Texture("planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
		Texture("textures/floor2/Stone_Floor_006_basecolor.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("textures/floor2/Stone_Floor_006_roughness.jpg", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
	};

	// Texture data
	Texture BoxTextures[]
	{
		Texture("textures/wood/Wood_025_basecolor.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("textures/wood/Wood_025_roughness.jpg", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
	};

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);

	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Crate light mesh
	Mesh light(lightVerts, lightInd, tex);

	// BOX
	Shader shaderProgram2("default.vert", "default.frag");
	std::vector <Vertex> verts2(BoxVertices, BoxVertices + sizeof(BoxVertices) / sizeof(Vertex));
	std::vector <GLuint> ind2(boxIndices, boxIndices + sizeof(boxIndices) / sizeof(GLuint));
	std::vector <Texture> tex2(BoxTextures, BoxTextures + sizeof(BoxTextures) / sizeof(Texture));
	// Create floor mesh
	Mesh Box(verts2, ind2, tex2);


	//light transform
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -2.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	//lightModel = glm::scale(lightModel, glm::vec3(1,1,1));

	//floor transform
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	objectModel = glm::scale(objectModel, glm::vec3(10, 10, 10));

	//box transform
	glm::vec3 objectPos2 = glm::vec3(0.5f, 0.0f, -2.0f);
	glm::mat4 objectModel2 = glm::mat4(1.0f);
	objectModel2 = glm::translate(objectModel2, objectPos2);
	objectModel2 = glm::scale(objectModel2, glm::vec3(2, 2, 2));

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	shaderProgram2.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel2));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.15f, 0.0f));
	
	//init music
	//SoundEngine->play2D("theme.mp3", true);

	//change cursor icon
	//GLFWcursor* crosshairCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	//glfwSetCursor(window, crosshairCursor);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.01f, 100.0f); //modify fov to zoom


		// Draws different meshes
		floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);
		Box.Draw(shaderProgram2, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	lightShader.Delete();
	shaderProgram2.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}