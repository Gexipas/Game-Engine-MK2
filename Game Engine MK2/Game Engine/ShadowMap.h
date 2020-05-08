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
	void ShadowPass();

};

inline void ShadowMap::Init()
{
	
}

inline void ShadowMap::ShadowPass()
{
	
}