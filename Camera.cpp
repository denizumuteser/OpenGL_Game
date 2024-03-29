#include"Camera.h"

bool cheat_mode = false;
int KEY_C_LAST_STATE = GLFW_RELEASE;
int KEY_LEFT_CLICK_LAST_STATE = GLFW_RELEASE;

//irrklang::ISoundEngine* SoundEngine2 = irrklang::createIrrKlangDevice();

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	// Sets new camera matrix
	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, bool* doFire, bool* cheatmode)
{
	*cheatmode = cheat_mode;
	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position.x += speed * Orientation.x;
		Position.z += speed * Orientation.z;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position.x += speed * -glm::normalize(glm::cross(Orientation, Up)).x;
		Position.z += speed * -glm::normalize(glm::cross(Orientation, Up)).z;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position.x += speed * -Orientation.x;
		Position.z += speed * -Orientation.z;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position.x += speed * glm::normalize(glm::cross(Orientation, Up)).x;
		Position.z += speed * glm::normalize(glm::cross(Orientation, Up)).z;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && cheat_mode)
	{
		Position += speed * Up;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && cheat_mode)
	{
		Position += speed * -Up;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.03f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		KEY_C_LAST_STATE = GLFW_PRESS;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && KEY_C_LAST_STATE == GLFW_PRESS)
	{
		if (cheat_mode)
		{
			cheat_mode = false;
			//init position and oriantation
			Orientation.y = 0.0f;
			Position.y = 0.2f;
			updateCollisionBox();

		}
		else
		{
			cheat_mode = true;
		}
		KEY_C_LAST_STATE = GLFW_RELEASE;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && KEY_LEFT_CLICK_LAST_STATE == GLFW_RELEASE)
	{
		//SoundEngine2->play2D("shot.mp3", false);
		*doFire = true;
		KEY_LEFT_CLICK_LAST_STATE = GLFW_PRESS;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		*doFire = false;
		KEY_LEFT_CLICK_LAST_STATE = GLFW_RELEASE;
	}

	// Handles mouse inputs
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		//freecam
		//if (cheat_mode)
		//{
			// Calculates upcoming vertical change in the Orientation
			glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
			// Decides whether or not the next vertical Orientation is legal or not
			if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				Orientation = newOrientation;
			}
		//}
		
		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
		
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Unhides cursor since camera is not looking around anymore
		//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}

/*
void Camera::InputsMinimap(GLFWwindow* window)
{
	if (!glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position.x += speed * Orientation.x;
		Position.z += speed * Orientation.z;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position.x += speed * -glm::normalize(glm::cross(Orientation, Up)).x;
		Position.z += speed * -glm::normalize(glm::cross(Orientation, Up)).z;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position.x += speed * -Orientation.x;
		Position.z += speed * -Orientation.z;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position.x += speed * glm::normalize(glm::cross(Orientation, Up)).x;
		Position.z += speed * glm::normalize(glm::cross(Orientation, Up)).z;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && cheat_mode)
	{
		Position += speed * Up;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && cheat_mode)
	{
		Position += speed * -Up;
		updateCollisionBox();
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 0.03f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 0.02f;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		KEY_C_LAST_STATE = GLFW_PRESS;
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE && KEY_C_LAST_STATE == GLFW_PRESS)
	{
		if (cheat_mode)
		{
			cheat_mode = false;
			//init position and oriantation
			Orientation.y = 0.0f;
			Position.y = 0.2f;
			updateCollisionBox();

		}
		else
		{
			cheat_mode = true;
		}
		KEY_C_LAST_STATE = GLFW_RELEASE;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && KEY_LEFT_CLICK_LAST_STATE == GLFW_RELEASE)
	{
		//SoundEngine2->play2D("shot.mp3", false);
		*doFire = true;
		KEY_LEFT_CLICK_LAST_STATE = GLFW_PRESS;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		//*doFire = false;
		KEY_LEFT_CLICK_LAST_STATE = GLFW_RELEASE;
	}

	// Handles mouse inputs
	if (glfwGetWindowAttrib(window, GLFW_FOCUSED))
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		//freecam
		//if (cheat_mode)
		//{
			// Calculates upcoming vertical change in the Orientation
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}
		//}

		// Rotates the Orientation left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));

	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Unhides cursor since camera is not looking around anymore
		//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
}
*/
void Camera::updateCollisionBox()
{
	minX = Position.x - 0.05f;
	maxX = Position.x + 0.05f;
	minY = Position.y - 0.1f;
	maxY = Position.y + 0.1f;
	minZ = Position.z - 0.05f;
	maxZ = Position.z + 0.05f;
}

bool Camera::checkCollision(float BminX, float BmaxX, float BminY, float BmaxY, float BminZ, float BmaxZ)
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
