

#ifndef SHADER_H
#define SHADER_H

// Dependency Includes
#include <gl/glew.h>
#include <glm/glm.hpp>

// Library Includes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

class shaderLoader
{
public:
	static shaderLoader& instance()
	{
		static shaderLoader* instance = new shaderLoader();
		return *instance;
	}

	~shaderLoader(void) {}

	GLuint createProgram(const char* shaderFilename)
	{
		GLuint shaderID;
		if (programList.count(std::string(shaderFilename)) == 0)
		{
			char vertexShaderFilename[256];
			strncpy_s(vertexShaderFilename, "resources/shaders/", sizeof(vertexShaderFilename));
			strncat_s(vertexShaderFilename, shaderFilename, sizeof(vertexShaderFilename));
			strncat_s(vertexShaderFilename, "VertexShader.txt", sizeof(vertexShaderFilename));

			char fragmentShaderFilename[256];
			strncpy_s(fragmentShaderFilename, "resources/shaders/", sizeof(fragmentShaderFilename));
			strncat_s(fragmentShaderFilename, shaderFilename, sizeof(fragmentShaderFilename));
			strncat_s(fragmentShaderFilename, "FragmentShader.txt", sizeof(fragmentShaderFilename));

			char geometryShaderFilename[256];
			strncpy_s(geometryShaderFilename, "resources/shaders/", sizeof(geometryShaderFilename));
			strncat_s(geometryShaderFilename, shaderFilename, sizeof(geometryShaderFilename));
			strncat_s(geometryShaderFilename, "GeometryShader.txt", sizeof(geometryShaderFilename));
			
			char tessControlShaderFilename[256];
			strncpy_s(tessControlShaderFilename, "resources/shaders/", sizeof(tessControlShaderFilename));
			strncat_s(tessControlShaderFilename, shaderFilename, sizeof(tessControlShaderFilename));
			strncat_s(tessControlShaderFilename, "TessControlShader.txt", sizeof(tessControlShaderFilename));

			char tessEvalShaderFilename[256];
			strncpy_s(tessEvalShaderFilename, "resources/shaders/", sizeof(tessEvalShaderFilename));
			strncat_s(tessEvalShaderFilename, shaderFilename, sizeof(tessEvalShaderFilename));
			strncat_s(tessEvalShaderFilename, "TessEvalShader.txt", sizeof(tessEvalShaderFilename));

			GLuint vertex = createShader(GL_VERTEX_SHADER, vertexShaderFilename);
			GLuint fragment = createShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
			GLuint geometry;
			GLuint tessControl;
			GLuint tessEval;

			bool GeometryBool = false;
			if (FILE* file = fopen(geometryShaderFilename, "r")) {
				fclose(file);
				GeometryBool = true;
				geometry = createShader(GL_GEOMETRY_SHADER, geometryShaderFilename);
			}

			bool TessControlBool = false;
			if (FILE* file = fopen(tessControlShaderFilename, "r")) {
				fclose(file);
				TessControlBool = true;
				tessControl = createShader(GL_TESS_CONTROL_SHADER, tessControlShaderFilename);
			}

			bool TessEvalBool = false;
			if (FILE* file = fopen(tessEvalShaderFilename, "r")) {
				fclose(file);
				TessEvalBool = true;
				tessEval = createShader(GL_TESS_EVALUATION_SHADER, tessEvalShaderFilename);
			}

			shaderID = glCreateProgram();
			glAttachShader(shaderID, vertex);
			glAttachShader(shaderID, fragment);
			if (GeometryBool)
			{
				glAttachShader(shaderID, geometry);
			}
			if (TessControlBool)
			{
				glAttachShader(shaderID, tessControl);
			}
			if (TessEvalBool)
			{
				glAttachShader(shaderID, tessEval);
			}
			glLinkProgram(shaderID);

			// Check for link errors
			int link_result = 0;
			glGetProgramiv(shaderID, GL_LINK_STATUS, &link_result);
			if (link_result == GL_FALSE)
			{
				std::string programName = vertexShaderFilename + *fragmentShaderFilename;
				printErrorDetails(false, shaderID, programName.c_str());
				return 0;
			}
			std::cout << "Program made ID: " << shaderID << " " << shaderFilename << std::endl;

			programList.insert(std::make_pair(std::string(shaderFilename), shaderID));
		}
		else
		{
			shaderID = programList[std::string(shaderFilename)];
			std::cout << "Program already made" << std::endl;
		}
		return shaderID;
	}
private:
	std::map<std::string, GLuint> shaderList;
	std::map<std::string, GLuint> programList;

	shaderLoader(void) {}

	GLuint createShader(GLenum shaderType, const char* shaderName)
	{
		GLuint shaderID;
		if (shaderList.count(std::string(shaderName)) == 0)
		{
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
			shaderList.insert(std::make_pair(std::string(shaderName), shaderID));
		}
		else
		{
			shaderID = shaderList[std::string(shaderName)];
			std::cout << "Shader already made" << std::endl;
		}
		return shaderID;
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
};

class Shader
{
public:
	unsigned int ID;

	//Shader(const char* vertexPath, const char* fragmentPath)
	//{
	//	// create later
	//}
	Shader()
	{
	}

	Shader(const char* shaderName)
	{
		ID = shaderLoader::instance().createProgram(shaderName);
	}

	void use()
	{
		glUseProgram(ID);
	}

	// utility uniform functions
	// ------------------------------------------------------------------------
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	// ------------------------------------------------------------------------
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
};
#endif

