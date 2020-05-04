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

	void Render();

	void SetupMesh();
	
	Shader program;
    Shader grass = Shader("Grass");
	unsigned int VAO, texture;
	std::vector<GLuint> indices;
	std::vector<vert> vertices;
    std::string m_texturePath;
    glm::vec3 m_position = glm::vec3(0);
};

inline void Mesh3D::Render()
{
    program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    glm::mat4 view = Camera::instance().GetViewMatrix();
    glm::mat4 projection = Camera::instance().cameraMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);


    program.setMat4("projection", projection);
    program.setMat4("view", view);
    program.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);

    // grass
    grass.use();

    grass.setMat4("projection", projection);
    grass.setMat4("view", view);
    grass.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);
}

inline void Mesh3D::SetupMesh()
{
    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vert), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);

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
    totalPath.append(m_texturePath);
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
    program.use();
    program.setInt("tex", 0);

}