#include <GL/glew.h>  
#include <GLFW/glfw3.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "utils.h"
#include "cubeMap.h"
#include "Terrain.h"
#include "Cube.h"
#include "Cloth.h"
#include "ParticleSystem.h"
#include "Compute.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void show_glfw_error(int error, const char* description);

GLFWwindow* InitWindow();
void processInput(GLFWwindow* window);
void processUpdate(GLFWwindow* window);
void processRender(GLFWwindow* window);

// settings
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool bLineRender = true;
bool bFullscreen = true;
bool bGrass = false;
bool bCamera = true;


bool bClothRun = false;


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Cube* cuba;
Terrain* terra;
//Cloth* test;
ParticleSystem* particles;
Compute* snow;


Shader program3D;
Shader programShadow;
Shader programGrass;

// shadows
glm::vec3 lightPos(-2.0f,5.0f, -2.0f);
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int depthMapFBO;
unsigned int depthMap;

int main()
{
	GLFWwindow* window = InitWindow();

	terra = new Terrain(100.0f,100.0f,200,200);
	cuba = new Cube(glm::vec3(0.0f, 3.0f, 0.0f));
	//test = new Cloth(20,20);
	//particles = new ParticleSystem(glm::vec3(0.0f, 4.0f, 0.0f),"awesomeface.png");
	snow = new Compute("snowflake.png");

	programShadow = Shader("Shadow");
	program3D = Shader("3D");
	programGrass = Shader("Grass");

	program3D.use();
	program3D.setInt("Texture", 0);
	program3D.setInt("ShadowMap", 1);
	glUseProgram(0);

	// configure depth map FBO
	// -----------------------
	
	
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// Update
		processUpdate(window);

		// input
		processInput(window);

		// render
		processRender(window);
		

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

	float speed = 10.0f;
	glm::vec2 dir = { 0.0f,0.0f };

	if (bCamera)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			Camera::instance().ProcessKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			Camera::instance().ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			Camera::instance().ProcessKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			Camera::instance().ProcessKeyboard(RIGHT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Camera::instance().ProcessKeyboard(UP, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			Camera::instance().ProcessKeyboard(DOWN, deltaTime);
		}
	}
	else
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			dir.y += 1;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			dir.y -= 1;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			dir.x -= 1;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			dir.x += 1;
		}
		if (!(dir.x == 0 && dir.y == 0))
		{
			if (Camera::instance().Position.x > 40)Camera::instance().Position.x = 40;
			if (Camera::instance().Position.x < -40)Camera::instance().Position.x = -40;
			if (Camera::instance().Position.z > 40)Camera::instance().Position.z = 40;
			if (Camera::instance().Position.z < -40)Camera::instance().Position.z = -40;

			glm::vec3 v = Camera::instance().Position + glm::normalize(Camera::instance().Front * dir.y + Camera::instance().Right * dir.x) * speed * deltaTime;

			Camera::instance().Position = terra->getPosition(v.x, v.z) + glm::vec3(0, 1, 0);			
		}
	}
	
}

void processUpdate(GLFWwindow* window)
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//particles->Update(deltaTime);

	//if (bClothRun)
	//{
	//	test->Update(deltaTime);
	//}
}

void processRender(GLFWwindow* window)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glCullFace(GL_FRONT);
	programShadow.use();
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 20.0f);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightVPMatrix = lightProjection * lightView;
	programShadow.setMat4("lightVPMatrix", lightVPMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	terra->Render(programShadow);
	cuba->Render(programShadow);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(0);
	glCullFace(GL_BACK);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	cubeMap::instance().Render();
	//test->Render();

	glm::mat4 projection = Camera::instance().CameraProjMatrix();
	glm::mat4 view = Camera::instance().CameraViewMatrix();
	glm::mat4 pv = projection * view;

	// Grass
	if (bGrass)
	{
		glDisable(GL_CULL_FACE);
		programGrass.use();
		programGrass.setMat4("pv", pv);
		programGrass.setMat4("view", view);
		programGrass.setVec3("cameraPos", Camera::instance().Position);
		terra->Render(programGrass);
		glUseProgram(0);
		glEnable(GL_CULL_FACE);
	}	
	
	// main Render
	program3D.use();
	
	program3D.setMat4("pv", pv);

	program3D.setMat4("lightVPMatrix", lightVPMatrix);
	program3D.setVec3("cameraPos",Camera::instance().Position);
	program3D.setVec3("lightPos", lightPos);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
		
	terra->Render(program3D);
	cuba->Render(program3D);

	glUseProgram(0);

	//particles->Render();
	snow->Render();
	
	// Render End

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		if (bLineRender == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			bLineRender = false;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			bLineRender = true;
		}
		//test->Reset();
	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		//if (bClothRun == true)
		//{
		//	bClothRun = false;
		//}
		//else
		//{
		//	bClothRun = true;
		//}
	}
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		if (bGrass == true)
		{
			bGrass = false;
		}
		else
		{
			bGrass = true;
		}
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (bCamera == true)
		{
			if (Camera::instance().Position.x > 40)Camera::instance().Position.x = 40;
			if (Camera::instance().Position.x < -40)Camera::instance().Position.x = -40;
			if (Camera::instance().Position.z > 40)Camera::instance().Position.z = 40;
			if (Camera::instance().Position.z < -40)Camera::instance().Position.z = -40;
		
			glm::vec3 v = Camera::instance().Position;
		
			Camera::instance().Position = terra->getPosition(v.x, v.z) + glm::vec3(0, 1, 0);
			bCamera = false;
		}
		else
		{			
			bCamera = true;
		}
		//test->Release();
	}
	//int w = test->widthNodes;
	//int h = test->heightNodes;
	//if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && w > 2) // left downsize
	//{
	//	test = new Cloth(w - 1, h);
	//}
	//if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && w < 50) // right upsize
	//{
	//	test = new Cloth(w + 1, h);
	//}
	//if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && h > 2) // down downsize
	//{
	//	test = new Cloth(w, h - 1);
	//}
	//if (key == GLFW_KEY_UP && action == GLFW_PRESS && h < 50) // up upsize
	//{
	//	test = new Cloth(w, h + 1);
	//}
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
	
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	
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

	//fullscreen
	//glfwSetWindowMonitor(window, bfullscreen ? glfwGetPrimaryMonitor() : NULL, 0, 0, SCR_WIDTH, SCR_HEIGHT, GLFW_DONT_CARE);

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