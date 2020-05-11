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
    void Render(Shader _program);
    glm::vec3 getPosition(float x, float y);
private:
    void createTerrain();
    void smoothTerrain();
    glm::vec3 getNormal(int x, int y);
    vert getVert(int x, int y);

    float m_width, m_height;
    int m_cellCountX, m_cellCountY;
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

    SetupMesh("sh_bottom.jpg");
}

inline void Terrain::Render(Shader _program)
{
    Draw(_program);
}

inline glm::vec3 Terrain::getPosition(float x, float z)
{
    float xCellSpace = m_width / (m_cellCountX - 1);
    float zCellSpace = m_height / (m_cellCountY - 1);

    // Transform from terrain local space to "cell" space.
    float c = (x + 0.5f * m_width) / xCellSpace;
    float d = (z + 0.5f * m_height) / zCellSpace;

    // Get the row and column we are in.
    int row = (int)floorf(d); // z
    int col = (int)floorf(c); // x

    // Grab the heights of the cell we are in.
    // A*--*B
    //  |\ |
    //  | \|
    // C*--*D
    float A = m_heightMap[row][col];
    float B = m_heightMap[row][col + 1];
    float C = m_heightMap[(row + 1)][col];
    float D = m_heightMap[(row + 1)][col + 1];

    // Where we are relative to the cell.
    float s = c - (float)col;
    float t = d - (float)row;

    // If upper triangle ABC.
    if (s + t <= 1.0f)
    {
        float uy = B - A;
        float vy = C - A;
        return glm::vec3(x, A + s * uy + t * vy,z);
    }
    else // lower triangle DCB.
    {
        float uy = C - D;
        float vy = B - D;
        return glm::vec3(x, D + (1.0f - s) * uy + (1.0f - t) * vy , z);
    }

}

inline void Terrain::createTerrain()
{
    for (unsigned int i = 0; i < m_heightMap.size() - 1; i++)
    {
        for (unsigned int j = 0; j < m_heightMap[i].size() - 1; j++)
        {
            vertices.push_back(getVert(i, j));
            vertices.push_back(getVert(i + 1, j + 1));
            vertices.push_back(getVert(i, j + 1));

            vertices.push_back(getVert(i, j));
            vertices.push_back(getVert(i + 1, j));
            vertices.push_back(getVert(i + 1, j + 1));
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
        2,
        -(2 * (Ydown - Yup) - Yupleft - Ydownright + Yup + Yleft) / ay));
}

inline vert Terrain::getVert(int x, int y)
{
    vert temp;
    temp.position = glm::vec3((float)x / m_heightMap[y].size() * m_width, m_heightMap[y][x], (float)y / m_heightMap.size() * m_height);
    temp.tex = glm::vec2((float)x / (float)m_heightMap[y].size(), (float)y / (float)m_heightMap.size());
    temp.normal = getNormal(x, y);
    return temp;
}