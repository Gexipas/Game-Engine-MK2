#pragma once

#include <vector>

#include "Shader.h"
#include "Mesh3D.h"

class Cloth
{
public:
	Cloth();
	~Cloth();
	void Render();
	void Update();
private:
	Shader programCloth;
	std::vector<float> m_positions;
	std::vector<int> m_indices;
	GLuint VAO, VBO, EBO;
};

inline Cloth::Cloth()
{
	programCloth = Shader("Cloth");
	int x = 10;
	int z = 10;

	for ( int i = 0; i < z; i++)//z
	{
		for ( int j = 0; j < x; j++)//x
		{
			m_positions.push_back(j);
			m_positions.push_back(0);
			m_positions.push_back(i);
		}
	}
	for ( int i = 0; i < z - 1; i++)//z
	{
		for ( int j = 0; j < x - 1; j++)//x
		{
			m_indices.push_back(i * z + j);
			m_indices.push_back((i + 1) * z + j + 1);
			m_indices.push_back((i) * z + j + 1);

			m_indices.push_back((i) * z + j);
			m_indices.push_back((i + 1) * z + j);
			m_indices.push_back((i + 1) * z + j + 1);
		}
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(float), &m_positions[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int), &m_indices[0], GL_DYNAMIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);

	glBindVertexArray(0);
}

inline void Cloth::Render()
{
	glDisable(GL_CULL_FACE);
	programCloth.use();
	glm::mat4 projection = Camera::instance().CameraProjMatrix();
	glm::mat4 view = Camera::instance().CameraViewMatrix();
	glm::mat4 pv = projection * view;
	glm::mat4 model = glm::mat4(1.0f);
	programCloth.setMat4("pv", pv);
	programCloth.setMat4("model", model);

	glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO); // vertices
	//glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), &m_positions[0], GL_DYNAMIC_DRAW);
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // indices
	//glBufferData(GL_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_DYNAMIC_DRAW);

	//glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
	//glDisableVertexAttribArray(0);

	glBindVertexArray(0);

	glUseProgram(0);
	glEnable(GL_CULL_FACE);
}

inline void Cloth::Update()
{

}