#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Mesh3D.h"
#include "Perlin.h"
#include "ShadowMap.h"

#include <stdlib.h> 

class Terrain : public Mesh3D
{
public:
    Terrain(float _width, float _height, int _cellCountX, int _cellCountY);
    void Render();
private:
    void createTerrain();
    void smoothTerrain();
    glm::vec3 getNormal(int x, int y);

    float m_width, m_height;
    int m_cellCountX, m_cellCountY;

    Shader grass = Shader("Grass");
    std::vector<std::vector<float>> m_heightMap;
};

inline Terrain::Terrain(float _width, float _height, int _cellCountX, int _cellCountY)
{
    m_width = _width;
    m_height = _height;
    m_position = glm::vec3(-m_width / 2.0f, 0.0f, -m_height / 2.0f);
    m_cellCountX = _cellCountX;
    m_cellCountY = _cellCountY;
    m_heightMap = Perlin::GetInstance().createNoise(m_cellCountX, m_cellCountY, 10, 1.4f);

    for (int i = 0; i < 15; i++)
    {
        smoothTerrain();
    }
    createTerrain();

    m_texturePath = "sh_bottom.jpg";
    SetupMesh();
    program = Shader("3D");
}

inline void Terrain::Render()
{
    // shadow pass
    ShadowMap::GetInstance().ShadowMapStart();

    ShadowMap::GetInstance().ShadowPass();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);

    ShadowMap::GetInstance().ShadowProgram.setMat4("model", model);

    Draw();

    glUseProgram(0);

    ShadowMap::GetInstance().ShadowMapEnd();

    program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, ShadowMap::GetInstance().depthMapTexture);

    program.setInt("ShadowMap", 1);

    glm::mat4 view = Camera::instance().CameraViewMatrix();
    glm::mat4 projection = Camera::instance().CameraProjMatrix();

    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, m_position);


    program.setMat4("lightVPMatrix", ShadowMap::GetInstance().lightVPMatrix);
    program.setMat4("projection", projection);
    program.setMat4("view", view);
    program.setMat4("model", model);

    program.setVec3("cameraPos", Camera::instance().Position);
    program.setVec3("lightPos", ShadowMap::GetInstance().lightPosition);

    Draw();

    glUseProgram(0);

    // grass
    //grass.use();
    //
    //grass.setMat4("projection", projection);
    //grass.setMat4("view", view);
    //grass.setMat4("model", model);
    //
    //Draw();
    //
    //glUseProgram(0);
}

inline void Terrain::createTerrain()
{
    for (unsigned int i = 0; i < m_heightMap.size(); i++)
    {
        for (unsigned int j = 0; j < m_heightMap[i].size(); j++)
        {
            vert temp;
            temp.position = glm::vec3((float)j/ m_heightMap[i].size()*m_width, m_heightMap[i][j], (float)i/ m_heightMap.size()*m_height);
            temp.tex = glm::vec2((float)j / (float)m_heightMap[i].size(), (float)i / (float)m_heightMap.size());
            temp.normal = getNormal(j, i);
            vertices.push_back(temp);
        }
    }
    for (unsigned int i = 0; i < m_heightMap.size() - 1; i++)
    {
        for (unsigned int j = 0; j < m_heightMap[i].size() - 1; j++)
        {
            indices.push_back(i * (int)m_heightMap[i].size() + j);
            indices.push_back((i + 1) * (int)m_heightMap[i].size() + j + 1);
            indices.push_back((i)* (int)m_heightMap[i].size() + j + 1);

            indices.push_back((i)* (int)m_heightMap[i].size() + j);
            indices.push_back((i + 1) * (int)m_heightMap[i].size() + j);
            indices.push_back((i + 1) * (int)m_heightMap[i].size() + j + 1);
        }
    }
}

inline void Terrain::smoothTerrain()
{
    for (unsigned int i = 0; i < m_heightMap.size(); i++)
    {
        for (unsigned int j = 0; j < m_heightMap[i].size(); j++)
        {
            float total = 0.0f;
            float totalNum = 1.0f;
            total += m_heightMap[i][j];

            if (j != 0)// left
            {
                total += m_heightMap[i][j - 1];
                totalNum += 1.0f;
            }
            if (j != m_heightMap[i].size() - 1)//right
            {
                total += m_heightMap[i][j + 1];
                totalNum += 1.0f;
            }
            if (i != m_heightMap.size() - 1)//bot
            {
                total += m_heightMap[i+1][j];
                totalNum += 1.0f;
                if (j != 0)//bot left
                {
                    total += m_heightMap[i + 1][j-1];
                    totalNum += 1.0f;
                }
                if (j != m_heightMap[i].size()-1)//bot right
                {
                    total += m_heightMap[i + 1][j + 1];
                    totalNum += 1.0f;
                }
            }
            if (i != 0)//top
            {
                total += m_heightMap[i - 1][j];
                totalNum += 1.0f;
                if (j != 0)//top left
                {
                    total += m_heightMap[i - 1][j - 1];
                    totalNum += 1.0f;
                }
                if (j != m_heightMap.size() - 1)//top right
                {
                    total += m_heightMap[i - 1][j + 1];
                    totalNum += 1.0f;
                }
            }
            m_heightMap[i][j] = total / totalNum;
        }
    }
}

inline glm::vec3 Terrain::getNormal(int x, int y)
{
    float ax = m_width / (m_cellCountX - 1);
    float ay = m_height / (m_cellCountY - 1);
    float Yleft = 0, Yright = 0;
    float Yup = 0, Ydown = 0;
    float Yupleft = 0, Ydownright = 0;
    if (x != 0)// left
    {
        Yleft = m_heightMap[y][x - 1];
    }
    if (x != m_heightMap[y].size() - 1)//right
    {
        Yright = m_heightMap[y][x + 1];
    }
    if (y != m_heightMap.size() - 1)//bot
    {
        Ydown = m_heightMap[y + 1][x];

        if (x != m_heightMap[y].size() - 1)//bot right
        {
            Ydownright = m_heightMap[y + 1][x + 1];
        }
    }
    if (y != 0)//top
    {
       Yup = m_heightMap[y - 1][x];

        if (x != 0)//top left
        {
            Yupleft = m_heightMap[y - 1][x - 1];
        }
    }

    return glm::normalize(glm::vec3(
        (2 * (Yleft - Yright) + Yupleft - Ydownright - Yup + Ydown) / ax, 
        6,
        (2 * (Ydown - Yup) - Yupleft - Ydownright + Yup + Yleft) / ay));
}