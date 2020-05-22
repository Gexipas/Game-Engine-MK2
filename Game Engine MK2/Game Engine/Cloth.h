#pragma once

#include <vector>
#include <random>

#include "Shader.h"
#include "Mesh3D.h"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist(-10.0f, 10.0f);


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
	Cloth(int _clothWidth, int _clothHeight);
	~Cloth();
	void Render();
	void Update(float _deltaTime);
	void Reset();
	void Release();

	int widthNodes = 20;//x
	int heightNodes = 20;//z
private:
	Shader programCloth;

	// variables
	float distanceWidth = 1.0f;
	float distanceHeight = 1.0f;
	float distanceDiag;
	
	float gravity = 0.2f;
	float windZ = 0.008f;
	float friction = 0.999f;

	float ground = -20.0f;
	glm::vec3 sphereCenter = glm::vec3(15.0f,-10.0f,15.0f);
	float sphereRadius = 10.0f;

	std::vector<clothPoint> m_positions;
	std::vector<clothSticks> m_sticks;
	std::vector<int> m_indices;
	GLuint VAO, VBO, EBO;
};

inline Cloth::Cloth(int _clothWidth, int _clothHeight)
{
	widthNodes = _clothWidth;
	heightNodes = _clothHeight;

	distanceDiag = sqrtf((distanceWidth * distanceWidth + distanceHeight * distanceHeight));

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
			m_indices.push_back(i * widthNodes + j);
			m_indices.push_back((i + 1) * widthNodes + j + 1);
			m_indices.push_back((i) *widthNodes + j + 1);

			m_indices.push_back((i) * widthNodes + j);
			m_indices.push_back((i + 1) * widthNodes + j);
			m_indices.push_back((i + 1) * widthNodes + j + 1);
		}
	}

	m_positions[0].staticPoint = true;
	m_positions[widthNodes-1].staticPoint = true;

	for (int i = 0; i < heightNodes; i++)//z
	{
		for (int j = 0; j < widthNodes; j++)//x
		{
			clothSticks temp;
			
			temp.p0 = i * widthNodes + j;
			temp.p1 = i * widthNodes + j + 1;

			if (j != widthNodes - 1)
			{
				temp.length = distanceWidth;
				m_sticks.push_back(temp);
			}						

			if (i != heightNodes - 1)
			{
				temp.p1 = (i + 1) * widthNodes + j;
				
				temp.length = distanceHeight;
				m_sticks.push_back(temp);
				if (j != widthNodes - 1)
				{
					temp.length = distanceDiag;
					temp.p1 = (i + 1) * widthNodes + j + 1;
					m_sticks.push_back(temp);

					temp.p0 = (i+1) * widthNodes + j;
					temp.p1 = i * widthNodes + j + 1;
					m_sticks.push_back(temp);
				}
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

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(0);

	glUseProgram(0);
	glEnable(GL_CULL_FACE);
}

inline void Cloth::Update(float _deltaTime)
{
	for (unsigned int i = 0; i < m_positions.size(); i++)
	{
		glm::vec3 v = m_positions[i].position - m_positions[i].prevPosition;

		m_positions[i].prevPosition = m_positions[i].position;

		if (m_positions[i].staticPoint != true)
		{
			float groundFriction = 1.0f;
			if (m_positions[i].position.y == ground)
			{
				groundFriction = 0.9f;
			}

			m_positions[i].position += (v * friction * groundFriction);
			m_positions[i].position.y -= gravity * _deltaTime; // gravity

			//m_positions[i].position.x += (dist(mt)/1000.0f); // windx
			//m_positions[i].position.y += (dist(mt)/1000.0f); // windy
			//m_positions[i].position.z -= windZ; // windz

			// self collision
			//for (unsigned int j = i + 1; j < m_positions.size(); j++)
			//{
			//	glm::vec3 vec = m_positions[i].prevPosition - m_positions[j].position;
			//	float distance = glm::length(vec);
			//
			//	if (distance <= 1.0f)
			//	{
			//		vec = glm::normalize(vec);
			//		float test = glm::dot(vec, m_positions[i].position) - glm::dot(vec, m_positions[j].position);
			//		if (test < 0)
			//		{
			//			m_positions[i].position -= vec * test;
			//			m_positions[j].position += vec * test;
			//		}
			//	}
			//}

			if (m_positions[i].position.y < ground)
			{
				m_positions[i].position.y = ground;
			}	

			// sphere
			glm::vec3 rad = m_positions[i].position - sphereCenter;
			if (glm::length(rad) < sphereRadius)
			{
				m_positions[i].position = sphereCenter + glm::normalize(rad) * sphereRadius;
			}
		}
	}

	for (unsigned int i = 0; i < m_sticks.size(); i++)
	{
		glm::vec3 d = m_positions[m_sticks[i].p1].position - m_positions[m_sticks[i].p0].position;

		float distance = glm::length(d);

		float difference = m_sticks[i].length - distance;

		float percent = difference / distance / 2;

		if (percent > 0) { percent = 0; }

		glm::vec3 offset = d * percent *0.9f;

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
	for (unsigned int i = 0; i < m_sticks.size(); i++)
	{
		glm::vec3 d = m_positions[m_sticks[i].p1].position - m_positions[m_sticks[i].p0].position;

		float distance = glm::length(d);

		float difference = m_sticks[i].length - distance;

		float percent = difference / distance / 2;

		if (percent > 0) { percent = 0; }

		glm::vec3 offset = d * percent * 0.9f;

		if (m_positions[m_sticks[i].p0].staticPoint == true)
		{
			m_positions[m_sticks[i].p1].position += offset * 2.0f;
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

inline void Cloth::Reset()
{
	for (int i = 0; i < heightNodes; i++)//z
	{
		for (int j = 0; j < widthNodes; j++)//x
		{
			m_positions[i * widthNodes + j].position = glm::vec3(j, 0.0f, i);
			m_positions[i * widthNodes + j].prevPosition = glm::vec3(j, 0.0f, i);
		}
	}
}

inline void Cloth::Release()
{
	if (m_positions[0].staticPoint == false)
	{
		m_positions[0].staticPoint = true;
		m_positions[widthNodes - 1].staticPoint = true;
	}
	else
	{
		m_positions[0].staticPoint = false;
		m_positions[widthNodes - 1].staticPoint = false;
	}	
}