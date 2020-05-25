#pragma once


#if !defined(__UTILS_H__)
#define __UTILS_H__

#include <random>

// Constants
const unsigned int SCR_WIDTH = (unsigned int)1920;
const unsigned int SCR_HEIGHT = (unsigned int)1080;

static float randomFloat()
{
	float r = (float)rand() / (float)RAND_MAX;
	return r;
}

#endif   // __UTILS_H__

