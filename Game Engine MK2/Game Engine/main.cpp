#include <GL/glew.h>  
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"
#include "utils.h"
#include "cubeMap.h"
#include "3Dshapes.h"

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void show_glfw_error(int error, const char* description);
GLFWwindow* InitWindow();

// settings

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool bToggle = true;

int scissorHeight = 150;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool mouseClick = false;

int main()
{
	GLFWwindow* window = InitWindow();

	cubeMap::instance();
		
	Cube cube(1.0f, 1.0f, 1.0f, glm::vec2(0.0f, 0.0f), 0.0f);
	
	CubeFog cubeFog(1.0f, 1.0f, 1.0f, glm::vec2(2.0f, 0.0f), 0.0f);
	
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// Update
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		if (cube.pointCollision(glm::vec2(lastX, lastY)))
		{
			cube.clickCube(mouseClick);
		}

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_SCISSOR_TEST);
		glScissor(0, scissorHeight, SCR_WIDTH, SCR_HEIGHT - 2*scissorHeight);
		
		cubeMap::instance().Render();
		cube.Render();
		cubeFog.Render();
		
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void show_glfw_error(int error, const char* description) {
	std::cout << "Error: " << description << '\n';
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Camera::instance().ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Camera::instance().ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Camera::instance().ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Camera::instance().ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		Camera::instance().ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		Camera::instance().ProcessKeyboard(DOWN, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		if (bToggle == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			bToggle = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			bToggle = true;
		}
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (scissorHeight == 150)
		{			
			scissorHeight = 0;
		}
		else
		{			
			scissorHeight = 150;
		}
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);
		firstMouse = false;
	}

	float xoffset = static_cast<float>(xpos) - lastX;
	float yoffset = lastY - static_cast<float>(ypos); 

	lastX = static_cast<float>(xpos);
	lastY = static_cast<float>(ypos);

	Camera::instance().ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::instance().ProcessMouseScroll(static_cast<float>(yoffset));
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	mouseClick = (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
}

GLFWwindow* InitWindow()
{
	glfwSetErrorCallback(show_glfw_error);

	if (!glfwInit()) {
		std::cout << "GLFW" << '\n';
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL - Ethan Griffin", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// mouse capture
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "GLEW " << glewGetErrorString(err) << '\n';
		glfwTerminate();
		exit(-1);
	}
	std::cout << glGetString(GL_VERSION) << '\n';
	
	glEnable(GL_DEPTH_TEST);

	return window;
}