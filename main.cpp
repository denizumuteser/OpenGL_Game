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

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);

	//floor transform
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	objectModel = glm::scale(objectModel, glm::vec3(10, 10, 10));

	glm::vec4 lightColor = glm::vec4(1);
	glm::vec3 lightPos = glm::vec3(0);

	shaderProgram.Activate(); //floor
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.2f, 0.0f));


	// build and compile shaders
	// -------------------------
	Shader modelShader("simple.vert", "simple.frag");

	// load models
	// -----------
	Model modelModel(("models/backpack/backpack.obj"));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(objectModel, objectPos);
	model = glm::scale(objectModel, glm::vec3(0.1f, 0.1f, 0.1f));
	modelShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.cameraMatrix));
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.53f, 0.80f, 0.92f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.01f, 100.0f); //modify fov to zoom

		// Draws different meshes
		floor.Draw(shaderProgram, camera);

		//model
		modelModel.Draw(modelShader, camera);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	modelShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}