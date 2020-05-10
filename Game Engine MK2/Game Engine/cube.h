#pragma once

#include "Mesh3D.h"

std::vector<vert> cubeVertices = {
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},{0.0f, 0.0f, -1.0f}}, // back face
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f},{0.0f, 0.0f, -1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f},{0.0f, 0.0f, -1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f},{0.0f, 0.0f, -1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f},{0.0f, 0.0f, -1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f},{0.0f, 0.0f, -1.0f}},
                                            
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f},{0.0f, 0.0f, 1.0f}}, // front face
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f},{0.0f, 0.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f},{0.0f, 0.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f},{0.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f},{0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f},{0.0f, 0.0f, 1.0f}},
                                            
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f},{-1.0f, 0.0f, 0.0f}}, // left face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f},{-1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f},{-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f},{-1.0f, 0.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f},{-1.0f, 0.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f},{-1.0f, 0.0f, 0.0f}},
                                              
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f},{1.0f, 0.0f, 0.0f}}, // right face
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f},{1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f},{1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f},{1.0f, 0.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f},{1.0f, 0.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f},{1.0f, 0.0f, 0.0f}},
                                            
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f},{0.0f, -1.0f, 0.0f}}, // bottom face
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f},{0.0f, -1.0f, 0.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f},{0.0f, -1.0f, 0.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f},{0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f},{0.0f, -1.0f, 0.0f}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f},{0.0f, -1.0f, 0.0f}},
                                             
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f},{0.0f, 1.0f, 0.0f}}, // top face
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f},{0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f},{0.0f, 1.0f, 0.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f},{0.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f},{0.0f, 1.0f, 0.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f},{0.0f, 1.0f, 0.0f}}
};

class Cube : public Mesh3D
{
public:
	Cube(glm::vec3 _position);
	~Cube() {}

	void Render(Shader _program);
};

inline Cube::Cube(glm::vec3 _position)
{
    m_position = _position;
    vertices = cubeVertices;
	SetupMesh("Brown.png");
}

inline void Cube::Render(Shader _program)
{
    Draw(_program);
}