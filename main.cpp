#include "main.h"
#include <random>

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
	std::vector <float> zombie_speeds;

	std::vector <Model> zombies;
	std::vector <Shader> zombiesShaders;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-30, 30);
	
	std::uniform_int_distribution<int> distribution2(10,20);

	for (int i = 0; i < number_of_zombies; i++)
	{
		zombiesShaders.push_back(Shader("default.vert", "default.frag"));
		zombies.push_back(Model("models/zombie/scene.gltf"));

		glm::vec3 test = glm::vec3(distribution(generator) / 10.0f, 0.085f, distribution(generator) / 10.0f);

		zombie_speeds.push_back(distribution2(generator) / 10.0f);

		zombie_positions.push_back(test);

		//std::cout << test.x << "/" << test.y << "/" << test.z << std::endl;

		zombiesShaders[i].Activate(); //zombie
		glUniform4f(glGetUniformLocation(zombiesShaders[i].ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(zombiesShaders[i].ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
		
	}



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
	viewOrtho = glm::scale(viewOrtho, glm::vec3(0.45f, 0.8f, 1.0f));

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
		//std::cout << zombie_quat.w << "/" << zombie_quat.x << "/" << zombie_quat.y << "/" << zombie_quat.z << "/" << std::endl;
		zombieModel.Draw(zombieShader, camera, glm::vec3(0.2f, 0.085f, 0.0f), glm::quat(zombie_quat), glm::vec3(0.007, 0.0070, 0.007));

		for (int i = 0; i < zombies.size(); i++)
		{
			glm::mat4 lookat2 = glm::lookAt(
				glm::vec3(zombie_positions[i].x, zombie_positions[i].y, -zombie_positions[i].z),
				glm::vec3(camera.Position.x, 0.085f, -camera.Position.z),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);

			glm::quat zombie_quat2 = glm::quat(lookat2) * glm::quat(glm::vec3(glm::radians(-90.0f), 0, 0));
			//std::cout << zombie_quat2.w << "/" << zombie_quat2.x << "/" << zombie_quat2.y << "/" << zombie_quat2.z << "/" << std::endl;
			//std::cout << camera.Position.x << "   " << camera.Position.z << std::endl;


			glm::vec3 directionToCamera = glm::normalize(camera.Position - zombie_positions[i]);
			//move zombies
			zombie_positions[i].x += directionToCamera.x * zombie_speeds[i] * 0.0005;
			zombie_positions[i].z += directionToCamera.z * zombie_speeds[i] * 0.0005;

			//delete zombie if too close
			if (( std::abs(camera.Position.x - zombie_positions[i].x) + std::abs(camera.Position.y - zombie_positions[i].y)  + std::abs(camera.Position.z - zombie_positions[i].z)) <= 0.25)
			{
				//CONVERT THIS TO PLAYER TAKING DAMAGE
				zombies.erase(zombies.begin() + i);
				//zombiesShaders[i].Delete();
				zombiesShaders.erase(zombiesShaders.begin() + i);
				zombie_positions.erase(zombie_positions.begin() + i);
				zombie_speeds.erase(zombie_speeds.begin() + i);
			}
			else
			{
				//update shading on zombies
				zombiesShaders[i].Activate(); //zombie
				glUniform4f(glGetUniformLocation(zombiesShaders[i].ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
				glUniform3f(glGetUniformLocation(zombiesShaders[i].ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
				zombies[i].Draw(zombiesShaders[i], camera, zombie_positions[i], glm::quat(zombie_quat2), glm::vec3(0.007, 0.0070, 0.007));
			}
		}

		//std::cout << zombies.meshes.size() << std::endl;

		//crosshair
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
	for (int i = 0; i < zombiesShaders.size(); i++)
	{
		zombiesShaders[i].Delete();
	}
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}