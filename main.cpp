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

	float wallsMinX = -4.9f;
	float wallsMaxX = 4.9f;
	float wallsMinY = -4.9f;
	float wallsMaxY = 4.9f;
	float wallsMinZ = -4.9f;
	float wallsMaxZ = 4.9f;

	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Store mesh data in vectors for the mesh
	std::vector <Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector <GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	// Crate light mesh
	Mesh light(lightVerts, lightInd, tex);

	//shaders
	//Shader mapShader("default.vert", "default.frag");
	Shader bulletShader("default.vert", "default.frag");

	//models

	//Model mapModel("models/map2/scene.gltf");
	Model bullet("models/bullet/scene.gltf");


	//light transform
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(-0.05f, 0.0f, -0.05f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	//floor transform
	glm::vec3 objectPos = glm::vec3(0.0f, -0.0f, 0.0f);
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
	//mapShader.Activate(); //map
	//glUniform4f(glGetUniformLocation(mapShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	//glUniform3f(glGetUniformLocation(mapShader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	
	bulletShader.Activate(); //crate
	glUniform4f(glGetUniformLocation(bulletShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(bulletShader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);

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
	unsigned int number_of_crates = 10;

	std::vector <Model> zombies;
	std::vector <Model> crates;
	std::vector <Model> bullets;


	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(-25, 25);
	
	std::uniform_int_distribution<int> distribution2(30,40);

	for (int i = 0; i < number_of_zombies; i++)
	{
		zombies.push_back(Model("models/zombie/scene.gltf"));
		zombies[i].shader = Shader("default.vert", "default.frag");
		zombies[i].position = glm::vec3(distribution(generator) / 10.0f, 0.085f, distribution(generator) / 10.0f);
		zombies[i].setCollisionBox(
			zombies[i].position.x - 0.05f,
			zombies[i].position.x + 0.05f,
			zombies[i].position.y - 0.1f,
			zombies[i].position.y + 0.1f,
			zombies[i].position.z - 0.05f,
			zombies[i].position.z + 0.05f
		);
		zombies[i].speed = (distribution2(generator) / 10.0f);
		zombies[i].shader.Activate();
		glUniform4f(glGetUniformLocation(zombies[i].shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(zombies[i].shader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
	}

	for (int ii = 0; ii < number_of_crates; ii++)
	{
		crates.push_back(Model("models/crate/scene.gltf"));
		crates[ii].shader = Shader("default.vert", "default.frag");
		crates[ii].position = glm::vec3(distribution(generator) / 10.0f, 0.085f, distribution(generator) / 10.0f);
		//crates[ii].updateCollisionBox();
		crates[ii].setCollisionBox(
			crates[ii].position.x - 0.1f,
			crates[ii].position.x + 0.1f,
			crates[ii].position.y - 0.1f,
			crates[ii].position.y + 0.1f,
			crates[ii].position.z - 0.1f + 0.1f,
			crates[ii].position.z + 0.1f + 0.1f
		);
		crates[ii].shader.Activate();
		glUniform4f(glGetUniformLocation(crates[ii].shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
		glUniform3f(glGetUniformLocation(crates[ii].shader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
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

	//Heart
	Shader HeartShader("ui.vert", "ui.frag");

	Texture textureHeart1 = Texture("textures/ui/heart-1.png", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture textureHeart2 = Texture("textures/ui/heart-2.png", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture textureHeart3 = Texture("textures/ui/heart-3.png", 0, GL_RGBA, GL_UNSIGNED_BYTE);

	unsigned int VBO3, VAO3, EBO3;
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &EBO3);

	glBindVertexArray(VAO3);

	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCrosshair), verticesCrosshair, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
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
	
	//End screen
	Shader EndScreenShader("ui.vert", "ui.frag");
	Texture endScreen = Texture("textures/ui/you-died.png", 0, GL_RGB, GL_UNSIGNED_BYTE);

	unsigned int VBO4, VAO4, EBO4;
	glGenVertexArrays(1, &VAO4);
	glGenBuffers(1, &VBO4);
	glGenBuffers(1, &EBO4);

	glBindVertexArray(VAO4);

	glBindBuffer(GL_ARRAY_BUFFER, VBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCrosshair), verticesCrosshair, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
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
	glm::mat4 viewOrthoCrosshair = glm::scale(viewOrtho, glm::vec3(0.45f, 0.8f, 1.0f));

	glm::mat4 viewOrthoHeart = glm::translate(viewOrtho, glm::vec3(0.85f, 0.9f, 0.0f));
	viewOrthoHeart = glm::scale(viewOrthoHeart, glm::vec3(1.35f, 0.8f, 1.0f));

	glm::mat4 viewOrthoEndScreen = glm::scale(viewOrtho, glm::vec3(10.0f, 10.0f, 1.0f));

	int playerHealth = 3;
	bool doFire = false;

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
			camera.Inputs(window, &doFire);
			if (doFire)
			{
				//fire a bullet
				Model tempBullet = Model("models/bullet/scene.gltf");
				tempBullet.shader = Shader("default.vert", "default.frag");
				tempBullet.position = camera.Position;
				tempBullet.setCollisionBox(
					tempBullet.position.x - 0.01f,
					tempBullet.position.x + 0.01f,
					tempBullet.position.y - 0.01f,
					tempBullet.position.y + 0.01f,
					tempBullet.position.z - 0.01f,
					tempBullet.position.z + 0.01f
				);
				//give speed
				tempBullet.speed = 2.0f;
				//give direction
				tempBullet.moveDirection = glm::normalize(camera.Orientation);
				//give rotation
				//tempBullet.rotation = glm::quat(glm::lookAt(
				//	tempBullet.position,
				//	(tempBullet.position + tempBullet.moveDirection),
				//	glm::vec3(1.0f, 0.0f, 0.0f)
				//)) * glm::quat(glm::vec3(glm::radians(-90.0f), 0, 0));
				tempBullet.rotation = glm::quat(glm::vec3(0, glm::radians(90.0f), 0)); //fix initial direction
				tempBullet.rotation *=  glm::quat(glm::inverse(glm::lookAt(
					glm::vec3(tempBullet.position.x, tempBullet.position.y, tempBullet.position.z),
					(glm::vec3(tempBullet.position.x, tempBullet.position.y, tempBullet.position.z) + glm::vec3(tempBullet.moveDirection.x, tempBullet.moveDirection.y, tempBullet.moveDirection.z)),
					glm::vec3(0.0f, 1.0f, 0.0f)
				)));
				//glm::vec3(glm::radians(-90.0f), 0, 0));

				//tempBullet.shader.Activate();
				//glUniform4f(glGetUniformLocation(tempBullet.shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
				//glUniform3f(glGetUniformLocation(tempBullet.shader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
				//add model to vector
				bullets.push_back(tempBullet);

				std::cout << "Fired at direction " << tempBullet.moveDirection.x << "|" << tempBullet.moveDirection.y<< "|" << tempBullet.moveDirection.z << std::endl;
				doFire = false;
			}
			
		}

		// Specify the color of the background
		glClearColor(0.53f, 0.80f, 0.92f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.01f, 100.0f); //modify fov to zoom

		// Draws meshes
		floor.Draw(shaderProgramFloor, camera, objectModel);
		walls.Draw(shaderProgramWalls, camera, objectModel);

		bullet.Draw(bulletShader, camera, glm::vec3(0.0f, -0.0f, 0.0f), glm::quat(0, 0, 0, 0), glm::vec3(0.1f, 0.1f, 0.1f));

		light.Draw(lightShader, camera, lightModel);

		//draw models
		//mapModel.Draw(mapShader, camera, glm::vec3(0.0f, -0.0f, 0.0f), glm::quat(0, 0, 0, 0), glm::vec3(0.1, 0.1, 0.1));
		
		//draw crates
		for (int k = 0; k < crates.size(); k++)
		{
			//update shading on crates
			crates[k].shader.Activate(); //crates
			//glUniform4f(glGetUniformLocation(crates[k].shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			//glUniform3f(glGetUniformLocation(crates[k].shader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
			crates[k].Draw(crates[k].shader, camera, crates[k].position, glm::quat(glm::vec3(0)), glm::vec3(0.003, 0.003, 0.003));
		}

		//draw bullets
		for (int kk = 0; kk < bullets.size(); kk++)
		{
			//update shading on crates
			bullets[kk].shader.Activate(); //bullets
			glUniform4f(glGetUniformLocation(bullets[kk].shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(bullets[kk].shader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
			bullets[kk].Draw(bullets[kk].shader, camera, bullets[kk].position, bullets[kk].rotation, glm::vec3(0.01f, 0.01f, 0.01f));
			bullets[kk].move(bullets[kk].moveDirection);
			//std::cout << bullets[kk].position.x << "|" << bullets[kk].position.z << std::endl;
		}
		/*
		camera.canMove = true;
		camera.updateCollisionBox();
		//collision check for camera vs walls
		if (!camera.checkCollision(wallsMinX, wallsMaxX, wallsMinY, wallsMaxY, wallsMinZ, wallsMaxZ))
		{//collided with other zombie
			std::cout << "collision" << std::endl;
			camera.canMove = false;
		}

		//collision check for camera vs crate
		for (int jjj = 0; jjj < crates.size(); jjj++)
		{
			if (crates[jjj].checkCollision(camera.minX, camera.maxX, camera.minY, camera.maxY, camera.minZ, camera.maxZ))
			{//collided with other zombie
				std::cout << "collision" << std::endl;
				camera.canMove = false;
				//camera.Position = prevPosCam;
				break;
			}
		}
		*/
		for (int i = 0; i < zombies.size(); i++)
		{
			bool zombieCanMove = true;
			glm::mat4 lookat2 = glm::lookAt(
				glm::vec3(zombies[i].position.x, zombies[i].position.y, -zombies[i].position.z),
				glm::vec3(camera.Position.x, 0.085f, -camera.Position.z),
				glm::vec3(0.0f, 1.0f, 0.0f)
			);

			glm::quat zombie_quat2 = glm::quat(lookat2) * glm::quat(glm::vec3(glm::radians(-90.0f), 0, 0));

			glm::vec3 directionToCamera = glm::normalize(camera.Position - zombies[i].position);
			//move zombies
			zombies[i].move(directionToCamera);
			//delete zombie if too close
			
			//check for collision
			camera.updateCollisionBox();
			if (zombies[i].checkCollision(camera.minX, camera.maxX, camera.minY, camera.maxY, camera.minZ, camera.maxZ))
			{ //collision check for camera vs zombie
				std::cout << "colide with camera" << std::endl;
				//Zombie dies from collison
				zombies[i].shader.Delete();
				zombies.erase(zombies.begin() + i);
				//player take damage
				playerHealth -= 1;
				continue;
				//zombieCanMove = false;
			}

			if (!zombies[i].checkCollision(wallsMinX, wallsMaxX, wallsMinY, wallsMaxY, wallsMinZ, wallsMaxZ))
			{ //collision check for walls vs zombie

				zombieCanMove = false;
			}


			//collision check for zombie vs crate
			
			for (int j = 0; j < zombies.size(); j++)
			{
				if (i != j)//not check yourself
				{
					if (zombies[i].checkCollision(zombies[j]))
					{//collided with other zombie
						zombieCanMove = false;
						break;
					} 
				}
				
			}
			//collision check for zombie vs crate
			for (int jj = 0; jj < crates.size(); jj++)
			{
				if (crates[jj].checkCollision(zombies[i]))
				{//collided with other zombie
					zombieCanMove = false;
					break;
				}
			}


			if (!zombieCanMove)
			{
				zombies[i].move(-directionToCamera);
				//zombies[i].move(glm::vec3(directionToCamera.z, -directionToCamera.y, -directionToCamera.x));
			}

			//update shading on zombies
			zombies[i].shader.Activate(); //zombie
			glUniform4f(glGetUniformLocation(zombies[i].shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
			glUniform3f(glGetUniformLocation(zombies[i].shader.ID, "lightPos"), lightPos2.x, lightPos2.y, lightPos2.z);
			zombies[i].Draw(zombies[i].shader, camera, zombies[i].position, glm::quat(zombie_quat2), glm::vec3(0.007, 0.0070, 0.007));
			
		}
		//dont render health when dead
		if (playerHealth > 0)
		{
			//crosshair
			textureCrosshair.Bind();

			// render crosshair
			CrosshairShader.Activate();
			glBindVertexArray(VAO2);
			glUniformMatrix4fv(glGetUniformLocation(CrosshairShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionOrtho));
			glUniformMatrix4fv(glGetUniformLocation(CrosshairShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelOrtho));
			glUniformMatrix4fv(glGetUniformLocation(CrosshairShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewOrthoCrosshair));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		//heart according to player health level
		if (playerHealth == 3)
		{
			textureHeart3.Bind();
		}
		else if (playerHealth == 2)
		{
			textureHeart2.Bind();
		}
		else if (playerHealth == 1)
		{
			textureHeart1.Bind();
		}
		else
		{
			//DIED NOW
		}
		//dont render health when dead
		if (playerHealth > 0)
		{
			// render heart
			HeartShader.Activate();
			glBindVertexArray(VAO3);
			glUniformMatrix4fv(glGetUniformLocation(HeartShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionOrtho));
			glUniformMatrix4fv(glGetUniformLocation(HeartShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelOrtho));
			glUniformMatrix4fv(glGetUniformLocation(HeartShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewOrthoHeart));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else
		{	//CURRENTLY DEAD
			// render ending screen
			endScreen.Bind();
			EndScreenShader.Activate();
			glBindVertexArray(VAO4);
			glUniformMatrix4fv(glGetUniformLocation(EndScreenShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projectionOrtho));
			glUniformMatrix4fv(glGetUniformLocation(EndScreenShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(modelOrtho));
			glUniformMatrix4fv(glGetUniformLocation(EndScreenShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(viewOrthoEndScreen));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgramFloor.Delete();
	shaderProgramWalls.Delete();
	lightShader.Delete();
	//mapShader.Delete();
	for (int i = 0; i < zombies.size(); i++)
	{
		zombies[i].shader.Delete();
	}
	for (int i = 0; i < crates.size(); i++)
	{
		crates[i].shader.Delete();
	}
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}