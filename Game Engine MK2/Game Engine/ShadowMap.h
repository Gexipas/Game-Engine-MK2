#pragma once

#include <iostream>
#include <GLFW/glfw3.h> 
#include <GL/glew.h>  

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.h"
#include "Shader.h"
#include "camera.h"

class ShadowMap
{
	/*-------------------Singleton----------------------*/
public:
	static ShadowMap& GetInstance()
	{
		static ShadowMap instance;
		return instance;
	}
	ShadowMap(ShadowMap const&) = delete;
	void operator= (ShadowMap const&) = delete;

private:
	ShadowMap() 
	{
		Init();
	};
	~ShadowMap() {};
	/*--------------------------------------------------*/

public:

	void Init();
	void ShadowMapStart();
	void ShadowMapEnd();
	void ShadowPass();

	Shader ShadowProgram = Shader("Shadow");
	GLuint depthMapFBO;
	GLuint depthMapTexture;

	glm::vec3 lightPosition = { 0.0f,0.0f,0.0f };
	glm::mat4 lightVPMatrix;
};

inline void ShadowMap::Init()
{
	// Depth Texture
	
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);

	glTexImage2D(GL_TEXTURE_2D, 
		0, //mipmaplevel
		GL_DEPTH_COMPONENT, //internal format
		SCR_WIDTH, //screen width
		SCR_HEIGHT, //screen height
		0, //border
		GL_DEPTH_COMPONENT, //color format
		GL_FLOAT, //data type
		NULL);

	glGenFramebuffers(1, &depthMapFBO); 
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	
	// attach depth texture as FBO's depth buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 
		GL_TEXTURE_2D, depthMapTexture, 0);

	//disable writes to color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind buffer

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "FB error, status: " << Status << std::endl;
	}
}

inline void ShadowMap::ShadowMapStart() 
{ 
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT); 
}

inline void ShadowMap::ShadowMapEnd()
{ 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

inline void ShadowMap::ShadowPass()
{
	ShadowProgram.use();

	glm::mat4 lightViewMatrix = glm::lookAt(
		lightPosition,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)); 
	
	lightVPMatrix = Camera::instance().CameraProjMatrix() 
		* lightViewMatrix;

	ShadowProgram.setMat4("lightVPMatrix", lightVPMatrix);
}