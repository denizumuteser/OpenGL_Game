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

	//shaders
	Shader mapShader("default.vert", "default.frag");
	Shader zombieShader("default.vert", "default.frag");
	Shader zombiesShader("instance.vert", "default.frag");

	//models
	
	Model mapModel("models/map2/scene.gltf");
	Model zombieModel("models/zombie/scene.gltf");

	//light transform
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(-0.05f, 0.0f, -0.05f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	//floor transform
	glm::vec3 objectPos = glm::vec3(0.0f, -0.11f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);
	objectModel = glm::scale(objectModel, glm::vec3(10, 10, 10));

	glm::vec3 lightPos2 = glm::vec3(0.00f, 0.05f, 0.0f);

	lightShader.Activate(); //light
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate(); //floor
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	mapShader.Activate(); //map
	glUniform4f(glGetUniformLocation(mapShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(mapShader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	zombieShader.Activate(); //zombie
	glUniform4f(glGetUniformLocation(zombieShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(zombieShader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	zombiesShader.Activate(); 	//zombie instance shader
	glUniform4f(glGetUniformLocation(zombiesShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(zombiesShader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.2f, 1.0f));
	
	//init music
	//SoundEngine->play2D("theme.mp3", true);
	//change cursor icon
	//GLFWcursor* crosshairCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	//glfwSetCursor(window, crosshairCursor);

	//instancing
	unsigned int number_of_zombies = 100;
	// Radius of circle around which zombies spawn
	float radius = 2.0f;
	// How much position deviate from the radius
	float radiusDeviation = 0.5f;
	// Holds all transformations for zombies
	std::vector <glm::mat4> instanceMatrix;

	for (unsigned int i = 0; i < number_of_zombies; i++)
	{
		// Generates x and y for the function x^2 + y^2 = radius^2 which is a circle
		float x = randf();
		float finalRadius = radius + randf() * radiusDeviation;
		float y = ((rand() % 2) * 2 - 1) * sqrt(1.0f - x * x);


		// Holds transformations before multiplying them
		glm::vec3 tempTranslation = glm::vec3(0.2f, 0.085f, 0.0f);
		glm::quat tempRotation = glm::quat(glm::vec3(glm::radians(-90.0f), 0, 0));
		glm::vec3 tempScale = glm::vec3(0.007, 0.0070, 0.007);

		// Makes the random distribution more even
		if (randf() > 0.5f)
		{
			// Generates a translation near a circle of radius "radius"
			tempTranslation = glm::vec3(y * finalRadius, randf(), x * finalRadius);
			//dont translate on y axis
			tempTranslation.y = 0.085f;

		}
		else
		{
			// Generates a translation near a circle of radius "radius"
			tempTranslation = glm::vec3(x * finalRadius, randf(), y * finalRadius);
			//dont translate on y axis
			tempTranslation.y = 0.085f;
		}
		// Generates random rotations
		//tempRotation = glm::quat(1.0f, randf(), randf(), randf());
		// Generates random scales
		//tempScale = 0.1f * glm::vec3(randf(), randf(), randf());
		
		// Initialize matrices
		glm::mat4 trans = glm::mat4(1.0f);
		glm::mat4 rot = glm::mat4(1.0f);
		glm::mat4 sca = glm::mat4(1.0f);

		// Transform the matrices to their correct form
		trans = glm::translate(trans, tempTranslation);
		rot = glm::mat4_cast(tempRotation);
		sca = glm::scale(sca, tempScale);

		// Push matrix transformation
		instanceMatrix.push_back(trans * rot * sca);
	}
	// Create the zombie model with instancing enabled
	Model zombies(("models/zombie/scene.gltf"), number_of_zombies, instanceMatrix);

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

		// Draws meshes
		floor.Draw(shaderProgram, camera, objectModel);
		light.Draw(lightShader, camera, lightModel);


		//convert zombie roation from euler degrees to euler radians to quaternion rotation
		glm::quat myquaternion = glm::quat(glm::vec3(glm::radians(-90.0f),0,0));
		
		//draw models
		mapModel.Draw(mapShader, camera, glm::vec3(0.0f, -0.0f, 0.0f), glm::quat(0, 0, 0, 0), glm::vec3(0.1, 0.1, 0.1));
		zombieModel.Draw(zombieShader, camera, glm::vec3(0.2f, 0.085f, 0.0f), glm::quat(myquaternion), glm::vec3(0.007, 0.0070, 0.007));

		//draw instanced models
		zombies.Draw(zombiesShader, camera);

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	lightShader.Delete();
	mapShader.Delete();
	zombieShader.Delete();
	zombiesShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}