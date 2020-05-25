#pragma once

#include <GLFW/glfw3.h> 
#include <vector>

#include "Particle.h"
#include "Shader.h"



class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 _position, const char* _texturePath);
	~ParticleSystem() {}

	void Render();
	void Update(float _deltaTime);
	glm::vec3 randVelocity();

	std::vector<Particle> m_particles;
	std::vector<glm::vec3> m_partPositions;
	glm::vec3 m_position;

private:
	int m_nParticles;
	GLuint VAO, VBO, texture;
	Shader programParticle;
};

inline ParticleSystem::ParticleSystem(glm::vec3 _position, const char* _texturePath)
{
	programParticle = Shader("Particle");
	m_position = _position;
	m_nParticles = 4000;
	
	for (int i = 0; i < m_nParticles; i++)
	{
		Particle temp(m_position, randVelocity(), randomFloat() + 0.2f, i);
		m_partPositions.push_back(temp.m_position);
		m_particles.push_back(temp);
	}
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_partPositions.size() * sizeof(glm::vec3), &m_partPositions[0], GL_STATIC_DRAW);

	// position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

inline void ParticleSystem::Render()
{
	glm::mat4 projection = Camera::instance().CameraProjMatrix();
	glm::mat4 view = Camera::instance().CameraViewMatrix();
	glm::mat4 pv = projection * view;

	glm::vec3 vQuad1, vQuad2;

	glm::vec3 vView = Camera::instance().Front;
	vView = glm::normalize(vView);

	vQuad1 = glm::cross(vView, Camera::instance().Up);
	vQuad1 = glm::normalize(vQuad1);

	vQuad2 = glm::cross(vView,vQuad1);
	vQuad2 = glm::normalize(vQuad2);

	programParticle.use();

	programParticle.setVec3("vQuad1", vQuad1);
	programParticle.setVec3("vQuad2", vQuad2);

	programParticle.setMat4("pv", pv);

	glActiveTexture(GL_TEXTURE0);
	programParticle.setInt("Texture", 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_partPositions.size(), &m_partPositions[0], GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, m_nParticles); 
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

inline void ParticleSystem::Update(float _deltaTime)
{
	for (int i = 0; i < m_nParticles; i++)
	{
		if (m_particles[i].m_duration < 0.0f) // reset on particle expire
		{
			m_particles[i].m_position = m_position;
			m_particles[i].m_velocity = randVelocity();
			m_particles[i].m_duration = randomFloat() + 0.2f;
			m_partPositions[i] = m_particles[i].m_position;
			m_particles[i].m_cameraDist = glm::distance(Camera::instance().Position, m_position);
		}
		else
		{
			m_particles[i].Update(_deltaTime);
			m_partPositions[i] = m_particles[i].m_position;
		}
	}
	std::sort(m_particles.begin(), m_particles.end(), [](Particle a, Particle b) {return a.m_cameraDist > b.m_cameraDist; });
}

inline glm::vec3 ParticleSystem::randVelocity()
{
	float height = 0.1f;
	float distance = 0.1f;
	return glm::vec3(distance * 2.0f * randomFloat() - distance,
					 height * 2.0f * randomFloat() + height,
					 distance * 2.0f * randomFloat() - distance);
}