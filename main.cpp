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
	Texture texturesWalls[]
	{
		Texture("textures/wall/Brick_Wall_019_BaseColor.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
		Texture("textures/wall/Brick_Wall_019_Roughness.jpg", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
	};
	Texture texturesFloor[]
	{
		Texture("textures/Floor/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("textures/Floor/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
	};

	// Generates Shader object using shaders default.vert and default.frag
	//floor
	Shader shaderProgramFloor("default.vert", "default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts(verticesFloor, verticesFloor + sizeof(verticesFloor) / sizeof(Vertex));
	std::vector <GLuint> ind(indicesFloor, indicesFloor + sizeof(indicesFloor) / sizeof(GLuint));
	std::vector <Texture> tex(texturesFloor, texturesFloor + sizeof(texturesFloor) / sizeof(Texture));
	// Create floor mesh
	Mesh floor(verts, ind, tex);

	//walls
	Shader shaderProgramWalls("default.vert", "default.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> verts2(verticesWalls, verticesWalls + sizeof(verticesWalls) / sizeof(Vertex));
	std::vector <GLuint> ind2(indicesWalls, indicesWalls + sizeof(indicesWalls) / sizeof(GLuint));
	std::vector <Texture> tex2(texturesWalls, texturesWalls + sizeof(texturesWalls) / sizeof(Texture));
	Mesh walls(verts2, ind2, tex2);


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
	objectModel = glm::scale(objectModel, glm::vec3(5, 5, 5));

	//wall transform
	glm::vec3 objectPos2 = glm::vec3(0.0f, -0.0f, 0.0f);
	glm::mat4 objectModel2 = glm::mat4(1.0f);
	objectModel2 = glm::translate(objectModel2, objectPos2);
	objectModel2 = glm::scale(objectModel2, glm::vec3(1, 1, 1));

	glm::vec3 lightPos2 = glm::vec3(0.00f, 0.05f, 0.0f);

	lightShader.Activate(); //light
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgramFloor.Activate(); //floor
	glUniform4f(glGetUniformLocation(shaderProgramFloor.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgramFloor.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	shaderProgramWalls.Activate(); //walls
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramWalls.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel2));
	glUniform4f(glGetUniformLocation(shaderProgramWalls.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgramWalls.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	mapShader.Activate(); //map
	glUniform4f(glGetUniformLocation(mapShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(mapShader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	zombieShader.Activate(); //zombie
	glUniform4f(glGetUniformLocation(zombieShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(zombieShader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	zombiesShader.Activate(); //zombie instance shader
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
	unsigned int number_of_zombies = 10;
	std::vector <glm::vec3> zombie_positions;
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
		zombie_positions.push_back(tempTranslation);
	}
	// Create the zombie model with instancing enabled
	Model zombies(("models/zombie/scene.gltf"), number_of_zombies, instanceMatrix);

	// Main while loop

	//glm::quat zombie_quat = glm::quat(glm::vec3(glm::radians(-90.0f), 0, 0));

	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

	//crosshair
	Shader CrosshairShader("ui.vert", "ui.frag");

	Texture textureCrosshair = Texture("textures/ui/crosshair.png", 0, GL_RGBA, GL_UNSIGNED_BYTE);

	unsigned int VBO2, VAO2, EBO2;
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCrosshair), verticesCrosshair, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCrosshair), indicesCrosshair, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glm::mat4 modelOrtho(1.0f);
	glm::mat4 viewOrtho(1.0f);
	glm::mat4 projectionOrtho(1.0f);

	//projectionOrtho = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), -1.0f, 10.0f);
	//viewOrtho = glm::translate(viewOrtho, glm::vec3(0.0f, 0.0f, -1.0f));
	viewOrtho = glm::scale(viewOrtho, glm::vec3(0.6f, 0.8f, 1.0f));

	while (!glfwWindowShouldClose(window))
	{

		//fps
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 60.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "FPS: " + FPS + " ms: " + ms;
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
			// Handles camera inputs
			camera.Inputs(window);
		}

		// Specify the color of the background
		//glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClearColor(0.53f, 0.80f, 0.92f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.01f, 100.0f); //modify fov to zoom

		// Draws meshes
		floor.Draw(shaderProgramFloor, camera, objectModel);
		walls.Draw(shaderProgramWalls, camera, objectModel);
		light.Draw(lightShader, camera, lightModel);


		//convert zombie roation from euler degrees to euler radians to quaternion rotation


		//draw models
		mapModel.Draw(mapShader, camera, glm::vec3(0.0f, -0.0f, 0.0f), glm::quat(0, 0, 0, 0), glm::vec3(0.1, 0.1, 0.1));
		//zombieModel.Draw(zombieShader, camera, glm::vec3(0.2f, 0.085f, 0.0f), glm::quat(myquaternion), glm::vec3(0.007, 0.0070, 0.007));

		//rotate zombie
		glm::mat4 lookat = glm::lookAt(
			glm::vec3(0.2f, 0.085f, 0.0f),
			glm::vec3(camera.Position.x, 0.085f, -camera.Position.z),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		
		glm::quat zombie_quat = glm::quat(lookat) * glm::quat(glm::vec3(glm::radians(-90.0f), 0, 0));

		zombieModel.Draw(zombieShader, camera, glm::vec3(0.2f, 0.085f, 0.0f), glm::quat(zombie_quat), glm::vec3(0.007, 0.0070, 0.007));

		/*
		for (int i = 0; i < instanceMatrix.size(); i++)
		{
			lookat = glm::lookAt(
				zombie_positions[i],
				glm::vec3(camera.Position.x, 0.085f, -camera.Position.z),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);
			// Initialize matrices
			glm::mat4 trans = glm::mat4(1.0f);
			glm::mat4 rot = glm::mat4(1.0f);
			glm::mat4 sca = glm::mat4(1.0f);

			// Transform the matrices to their correct form
			trans = glm::translate(trans, glm::vec3(0,10,0));
			rot = glm::mat4_cast(glm::quat(lookat));
			//sca = glm::scale(sca, tempScale);
			zombies.instanceMatrix[i] = (trans * rot * sca);
		}
		//zombie_quat = glm::quat(angle);
		*/
		//draw instanced models
		zombies.Draw(zombiesShader, camera);

		//crosshair
		// bind Texture
		textureCrosshair.Bind();

		// render container
		CrosshairShader.Activate();
		glBindVertexArray(VAO2);
		glUniformMatrix4fv(glGetUniformLocation(CrosshairShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionOrtho));
		glUniformMatrix4fv(glGetUniformLocation(CrosshairShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelOrtho));
		glUniformMatrix4fv(glGetUniformLocation(CrosshairShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewOrtho));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgramFloor.Delete();
	shaderProgramWalls.Delete();
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