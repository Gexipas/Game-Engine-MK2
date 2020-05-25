#pragma once

#include <GLFW/glfw3.h> 
#include <vector>

#include "Particle.h"
#include "Shader.h"

#define NUM_PARTICLES 128*20000

void printErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(isShader == true) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);
	std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << name << std::endl;
	std::cout << &log[0] << std::endl;
}

std::string readShaderFile(const char* filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

GLuint createShader(GLenum shaderType, const char* shaderName)
{
	GLuint shaderID;
	std::string shaderSourceCode = readShaderFile(shaderName);
	const char* shaderCode = shaderSourceCode.c_str();
	const int shaderCodeSize = (const int)shaderSourceCode.size();
	shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &shaderCode, &shaderCodeSize);
	glCompileShader(shaderID);

	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		printErrorDetails(true, shaderID, shaderName);
		return 0;
	}
	std::cout << "Shader made ID: " << shaderID << " " << shaderName << std::endl;
	return shaderID;
}

class Compute
{
public:
	Compute();
	~Compute() {}

	void Render();

	std::vector<glm::vec4> initialPosition;
	std::vector<glm::vec4> initialVelocity;
	GLuint posVbo, velVbo, initVelVbo, particleVao;
	GLuint computeProgram;
	Shader renderProgram;

private:
};

inline Compute::Compute()
{
	renderProgram = Shader("Snow");

	computeProgram = glCreateProgram();

	char computeShaderFilename[256];
	strncpy_s(computeShaderFilename, "resources/shaders/", sizeof(computeShaderFilename));
	strncat_s(computeShaderFilename, "ComputeShader.txt", sizeof(computeShaderFilename));

	GLuint computeShader = createShader(GL_COMPUTE_SHADER, computeShaderFilename);

	glAttachShader(computeProgram, computeShader);
	glLinkProgram(computeProgram);

	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		initialPosition.push_back(glm::vec4(0.0f, 0.0f, 0.0f, randomFloat() + 0.125f));
		float height = 0.1f;
		float distance = 0.1f;
		initialVelocity.push_back(glm::vec4(distance * 2.0f * randomFloat() - distance,
			height * 2.0f * randomFloat() + height,
			distance * 2.0f * randomFloat() - distance,
		    randomFloat() + 0.125f));
	}

	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, initialPosition.size() * sizeof(glm::vec4), &initialPosition[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, posVbo);

	glGenBuffers(1, &velVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, initialVelocity.size() * sizeof(glm::vec4), &initialVelocity[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, velVbo);

	glGenBuffers(1, &initVelVbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, initVelVbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, initialVelocity.size() * sizeof(glm::vec4), &initialVelocity[0], GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, initVelVbo);

	glGenVertexArrays(1, &particleVao);
	glBindVertexArray(particleVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

inline void Compute::Render()
{
	glUseProgram(computeProgram);

	glDispatchCompute(NUM_PARTICLES / 128, 1, 1);

	glMemoryBarrier(GL_ALL_BARRIER_BITS);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	renderProgram.use();

	glm::mat4 pv = Camera::instance().CameraProjMatrix() * Camera::instance().CameraViewMatrix();
	renderProgram.setMat4("pv", pv);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, NULL, 0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_POINTS, 0, NUM_PARTICLES);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
}

