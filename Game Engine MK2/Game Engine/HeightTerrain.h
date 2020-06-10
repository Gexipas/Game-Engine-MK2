#pragma once

#include <stdlib.h> 
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

#include "Mesh3D.h"
#include "ShadowMap.h"

class HeightTerrain : public Mesh3D
{
public:
    HeightTerrain(const char* _filename);
    void Render(Shader _program);
private:
    vert getVert(int x, int z);

    float m_width, m_height;
    unsigned int heighttexture;
    unsigned int rivertexture;
    int Xterrain;
    int Zterrain;
};

inline HeightTerrain::HeightTerrain(const char* _filename)
{
    m_width = 600.0f;
    m_height = 600.0f;
    m_position = glm::vec3(m_width-500, 0.0f, -m_height / 2.0f);

    Xterrain = 300;
    Zterrain = 300;
    
    for (int i = 0; i < Xterrain - 1; i++)
    {
        for (int j = 0; j < Zterrain - 1; j++)
        {
            vertices.push_back(getVert(i, j));
            vertices.push_back(getVert(i, j + 1));
            vertices.push_back(getVert(i + 1, j + 1));
            vertices.push_back(getVert(i + 1, j));
        }
    }

    SetupMesh("default.jpg");

    heighttexture = SetupTex(_filename);
    rivertexture = SetupTex("river.png");
}

inline void HeightTerrain::Render(Shader _program)
{

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, heighttexture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, rivertexture);
    Draw(_program);
}


inline vert HeightTerrain::getVert(int x, int z)
{
    vert temp;
    temp.position = glm::vec3((float)x / (float)Xterrain * m_width, 0, (float)z / (float)Zterrain * m_height);
    temp.tex = glm::vec2((float)x / (float)Xterrain, (float)z / (float)Zterrain);
    temp.normal = glm::vec3(0);
    return temp;
}
