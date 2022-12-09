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

	//model
	Shader modelShader("default.vert", "default.frag");
	//model2
	Shader modelShader2("default.vert", "default.frag");
	//Model ourModel("models/backpack/backpack.obj");
	Model ourModel("models/map2/scene.gltf");
	//Model ourModel("models/crate/scene.gltf");
	Model ourModel2("models/zombie/scene.gltf");
	//light transform
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -2.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	//lightModel = glm::scale(lightModel, glm::vec3(1,1,1));

	//floor transform
	glm::vec3 objectPos = glm::vec3(0.0f, -0.11f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	objectModel = glm::scale(objectModel, glm::vec3(10, 10, 10));

	//model
	glm::vec3 objectPos3 = glm::vec3(-0.2f, -0.1f, -2.0f);
	//glm::vec3 objectPos3 = glm::vec3(-0.2f, 0.1f, -2.0f);
	glm::mat4 objectModel3 = glm::mat4(1.0f);
	objectModel3 = glm::translate(objectModel3, objectPos3);
	//objectModel3 = glm::scale(objectModel3, glm::vec3(0.008, 0.008, 0.008)); //zombie
	objectModel3 = glm::scale(objectModel3, glm::vec3(0.1, 0.1, 0.1)); //crate
	//objectModel3 = glm::scale(objectModel3, glm::vec3(0.1, 0.1, 0.1));
	//objectModel3 = glm::rotate(objectModel3, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	objectModel3 = glm::translate(objectModel3, glm::vec3(-0.5f, 1.0f, -2.0f));
	
	//model2
	glm::mat4 objectModel4 = glm::mat4(1.0f);
	objectModel4 = glm::translate(objectModel4, glm::vec3(-0.5f, 0.085f, -2.0f));
	objectModel4 = glm::scale(objectModel4, glm::vec3(0.0070, 0.0070, 0.0070)); //zombie
	objectModel4 = glm::rotate(objectModel4, glm::radians(-90.0f), glm::vec3(1, 0, 0));
	//objectModel4 = glm::translate(objectModel4, glm::vec3(-0.5f, 0.1f, -2.0f));

	glm::vec3 lightPos2 = glm::vec3(0.05f, 0.05f, -2.0f);

	lightShader.Activate(); //light
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate(); //floor
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	modelShader.Activate(); //model
	glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel3));
	glUniform4f(glGetUniformLocation(modelShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(modelShader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	modelShader2.Activate(); //model2
	glUniformMatrix4fv(glGetUniformLocation(modelShader2.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel4));
	glUniform4f(glGetUniformLocation(modelShader2.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(modelShader2.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.2f, 0.0f));
	
	//init music
	//SoundEngine->play2D("theme.mp3", true);
	//change cursor icon
	//GLFWcursor* crosshairCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	//glfwSetCursor(window, crosshairCursor);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(0.53f, 0.80f, 0.92f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.01f, 100.0f); //modify fov to zoom

		// Draws different meshes
		floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);
		//Box.Draw(shaderProgram2, camera);
		ourModel.Draw(modelShader, camera);
		ourModel2.Draw(modelShader2, camera);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	lightShader.Delete();
	modelShader.Delete();
	modelShader2.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}