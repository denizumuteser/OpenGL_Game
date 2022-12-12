#ifndef MAIN_H
#define MAIN_H
//#include "Mesh.h"
#include "Model.h"
//#include <irrklang/irrKlang.h>

//music
//irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

const unsigned int width = 1200;
const unsigned int height = 9.0 / 16.0 * width;
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Vertices coordinates
Vertex vertices[] = //floor
{ //               COORDINATES           /            NORMALS          /       COLOR        //           TexCoord         /
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1), glm::vec2(0.0f, 4.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1), glm::vec2(4.0f, 4.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1), glm::vec2(4.0f, 0.0f)}
};

Vertex vertices2[] = //wall
{ //               COORDINATES           /            NORMALS          /       COLOR        //           TexCoord         /
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1), glm::vec2(2.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.25f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1), glm::vec2(0.0f, 0.5f)},
	Vertex{glm::vec3(-1.0f, 0.25f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1), glm::vec2(2.0f, 0.5f)},
	
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1), glm::vec2(2.0f, 0.0f)},
	Vertex{glm::vec3(1.0f, 0.25f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1), glm::vec2(0.0f, 0.5f)},
	Vertex{glm::vec3(1.0f, 0.25f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1), glm::vec2(2.0f, 0.5f)},

	Vertex{glm::vec3(-1.0f, 0.0f,  -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1), glm::vec2(2.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.25f,  -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1), glm::vec2(0.0f, 0.5f)},
	Vertex{glm::vec3(1.0f, 0.25f,  -1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1), glm::vec2(2.0f, 0.5f)},

	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1), glm::vec2(2.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.25f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1), glm::vec2(0.0f, 0.5f)},
	Vertex{glm::vec3(1.0f, 0.25f,  1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1), glm::vec2(2.0f, 0.5f)},
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
};

GLuint indices2[] =
{
	0, 1, 2,
	1, 2, 3,

	5,4,7,
	4,7,6,

	8, 9, 11,
	8,10, 11,

	12,13,15,
	12,14,15

};

Vertex lightVertices[] =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.00f, -0.0f,  0.1f)},
	Vertex{glm::vec3(-0.00f, -0.0f, -0.00f)},
	Vertex{glm::vec3(0.1f, -0.0f, -0.00f)},
	Vertex{glm::vec3(0.1f, -0.0f,  0.1f)},
	Vertex{glm::vec3(-0.00f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.00f,  0.1f, -0.0f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.00f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

Vertex BoxVertices[] =
{ //     COORDINATES     //normal // color //texture
	Vertex{glm::vec3(-0.00f, -0.0f,  0.1f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1),glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.00f, -0.0f, -0.00f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1),glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(0.1f, -0.0f, -0.00f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1),glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.1f, -0.0f,  0.1f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1),glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(-0.00f,  0.1f,  0.1f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1),glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.00f,  0.1f, -0.0f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1),glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.00f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1),glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(1),glm::vec2(1.0f, 1.0f)},
};

GLuint boxIndices[] =
{
	0, 1, 2, //bottom front
	0, 2, 3, //bottom back
	0, 4, 7, //rear left
	0, 3, 7, //rear right
	3, 6, 7, //right top
	2, 3, 6, //right bottom
	2, 5, 6, //front right
	1, 2, 5, //front left
	1, 4, 5, //left top
	0, 1, 4, //left bottom
	4, 5, 6, //top front
	4, 6, 7 //top back
};



GLFWwindow* initilizeWindow()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);
	return window;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
#endif