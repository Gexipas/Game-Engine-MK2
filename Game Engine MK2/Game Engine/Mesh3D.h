#pragma once

#include "shader.h"
#include "camera.h"
#include "stb_image.h"

struct vert
{
	glm::vec3 position;
	glm::vec2 tex;
    glm::vec3 normal;
};

class Mesh3D
{
public:
	Mesh3D() {}
	~Mesh3D() {}

    void Draw(Shader _program);
	void SetupMesh(const char* _texturePath);
	
	unsigned int VAO, texture;
	std::vector<vert> vertices;
    glm::vec3 m_position = glm::vec3(0);
};

inline void Mesh3D::Draw(Shader _program)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);

    _program.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_PATCHES, 0, vertices.size());
    glBindVertexArray(0);
}

inline void Mesh3D::SetupMesh(const char* _texturePath)
{
    unsigned int VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vert), &vertices[0], GL_STATIC_DRAW);
    
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)0);
    // texture attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)offsetof(vert, tex));
    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vert), (GLvoid*)offsetof(vert, normal));

    glBindVertexArray(0);

    std::string totalPath = ".//resources//textures//";
    totalPath.append(_texturePath);
    char const* path = totalPath.c_str();

    glGenTextures(1, &texture);

    int width, height, nrComponents;
   // stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(totalPath.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}