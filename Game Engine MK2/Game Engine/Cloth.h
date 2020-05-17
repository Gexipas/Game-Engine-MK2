#pragma once

#include <vector>

#include "Shader.h"
#include "Mesh3D.h"

struct clothPoint
{
	glm::vec3 position;
	glm::vec3 prevPosition;
	bool staticPoint = false;
};

struct clothSticks
{
	int p0;
	int p1;
	float length;
};

class Cloth
{
public:
	Cloth();
	~Cloth();
	void Render();
	void Update();
private:
	Shader programCloth;
	int widthNodes = 10;//x
	int heightNodes = 10;//z
	float gravity = 0.001f;
	float windZ = 0.005f;
	std::vector<clothPoint> m_positions;
	std::vector<clothSticks> m_sticks;
	std::vector<int> m_indices;
	GLuint VAO, VBO, EBO;
};

inline Cloth::Cloth()
{
	programCloth = Shader("Cloth");

	for ( int i = 0; i < heightNodes; i++)//z
	{
		for ( int j = 0; j < widthNodes; j++)//x
		{
			clothPoint temp;
			temp.position = glm::vec3(j, 0.0f, i);
			temp.prevPosition = glm::vec3(j, 0.0f, i);
			m_positions.push_back(temp);
		}
	}
	for ( int i = 0; i < heightNodes - 1; i++)//z
	{
		for ( int j = 0; j < widthNodes - 1; j++)//x
		{
			m_indices.push_back(i * heightNodes + j);
			m_indices.push_back((i + 1) * heightNodes + j + 1);
			m_indices.push_back((i) *heightNodes + j + 1);

			m_indices.push_back((i) *heightNodes + j);
			m_indices.push_back((i + 1) * heightNodes + j);
			m_indices.push_back((i + 1) * heightNodes + j + 1);
		}
	}

	m_positions[0].staticPoint = true;
	m_positions[widthNodes-1].staticPoint = true;

	for (int i = 0; i < heightNodes; i++)//z
	{
		for (int j = 0; j < widthNodes; j++)//x
		{
			clothSticks temp;
			temp.length = 1.0f;
			temp.p0 = i * heightNodes + j;
			temp.p1 = i * heightNodes + j + 1;

			if (j != widthNodes - 1)
			{
				m_sticks.push_back(temp);
			}				
			
			temp.p1 = (i + 1) * heightNodes + j;

			if (i != heightNodes - 1)
			{
				m_sticks.push_back(temp);
			}
		}
	}
	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(clothPoint), &m_positions[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int), &m_indices[0], GL_DYNAMIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(clothPoint), (GLvoid*)0);

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

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(clothPoint), &m_positions[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(int), &m_indices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);

	glBindVertexArray(0);

	glUseProgram(0);
	glEnable(GL_CULL_FACE);
}

inline void Cloth::Update()
{
	for (unsigned int i = 0; i < m_positions.size(); i++)
	{
		glm::vec3 v = m_positions[i].position - m_positions[i].prevPosition;

		m_positions[i].prevPosition = m_positions[i].position;

		if (m_positions[i].staticPoint != true)
		{
			m_positions[i].position += v;
			m_positions[i].position.y -= gravity; // gravity
			m_positions[i].position.z -= windZ; // wind
		}

	}

	for (unsigned int i = 0; i < m_sticks.size(); i++)
	{
		glm::vec3 d = m_positions[m_sticks[i].p1].position - m_positions[m_sticks[i].p0].position;

		float distance = glm::length(d);

		float difference = m_sticks[i].length - distance;

		float percent = difference / distance / 2;

		if (percent > 0) { percent = 0; }

		glm::vec3 offset = d * percent;

		if (m_positions[m_sticks[i].p0].staticPoint == true)
		{
			m_positions[m_sticks[i].p1].position += offset*2.0f;
		}
		else if (m_positions[m_sticks[i].p1].staticPoint == true)
		{
			m_positions[m_sticks[i].p0].position -= offset * 2.0f;
		}
		else if (m_positions[m_sticks[i].p1].staticPoint == true && 
				 m_positions[m_sticks[i].p0].staticPoint == true)
		{
		}
		else
		{
			m_positions[m_sticks[i].p0].position -= offset;
			m_positions[m_sticks[i].p1].position += offset;
		}

		
	}
}