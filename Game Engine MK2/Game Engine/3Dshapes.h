#pragma once

#include "2Dshapes.h"
#include "camera.h"

std::vector<glm::vec4> vertices = {
       { -0.5f, -0.5f, -0.5f, 1.0f},
       {  0.5f, -0.5f, -0.5f, 1.0f},
       {  0.5f,  0.5f, -0.5f, 1.0f},
       {  0.5f,  0.5f, -0.5f, 1.0f},
       { -0.5f,  0.5f, -0.5f, 1.0f},
       { -0.5f, -0.5f, -0.5f, 1.0f},
       { -0.5f, -0.5f,  0.5f, 1.0f},
       {  0.5f, -0.5f,  0.5f, 1.0f},
       {  0.5f,  0.5f,  0.5f, 1.0f},
       {  0.5f,  0.5f,  0.5f, 1.0f},
       { -0.5f,  0.5f,  0.5f, 1.0f},
       { -0.5f, -0.5f,  0.5f, 1.0f},
       { -0.5f,  0.5f,  0.5f, 1.0f},
       { -0.5f,  0.5f, -0.5f, 1.0f},
       { -0.5f, -0.5f, -0.5f, 1.0f},
       { -0.5f, -0.5f, -0.5f, 1.0f},
       { -0.5f, -0.5f,  0.5f, 1.0f},
       { -0.5f,  0.5f,  0.5f, 1.0f},
       {  0.5f,  0.5f,  0.5f, 1.0f},
       {  0.5f,  0.5f, -0.5f, 1.0f},
       {  0.5f, -0.5f, -0.5f, 1.0f},
       {  0.5f, -0.5f, -0.5f, 1.0f},
       {  0.5f, -0.5f,  0.5f, 1.0f},
       {  0.5f,  0.5f,  0.5f, 1.0f},
       { -0.5f, -0.5f, -0.5f, 1.0f},
       {  0.5f, -0.5f, -0.5f, 1.0f},
       {  0.5f, -0.5f,  0.5f, 1.0f},
       {  0.5f, -0.5f,  0.5f, 1.0f},
       { -0.5f, -0.5f,  0.5f, 1.0f},
       { -0.5f, -0.5f, -0.5f, 1.0f},
       { -0.5f,  0.5f, -0.5f, 1.0f},
       {  0.5f,  0.5f, -0.5f, 1.0f},
       {  0.5f,  0.5f,  0.5f, 1.0f},
       {  0.5f,  0.5f,  0.5f, 1.0f},
       { -0.5f,  0.5f,  0.5f, 1.0f},
       { -0.5f,  0.5f, -0.5f, 1.0f},
};

float cubeVertices[] = {
    
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // back face
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,    
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // front face
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // left face
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // right face
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom face
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f
};

bool trianglePoint(glm::vec2 _point, glm::vec2 _p0, glm::vec2 _p1, glm::vec2 _p2)
{
    float calc = 1 / (-_p1.y * _p2.x + _p0.y * (-_p1.x + _p2.x) + _p0.x * (_p1.y - _p2.y) + _p1.x * _p2.y);
    float s = calc * (_p0.y * _p2.x - _p0.x * _p2.y + (_p2.y - _p0.y) * _point.x + (_p0.x - _p2.x) * _point.y);
    float t = calc * (_p0.x * _p1.y - _p0.y * _p1.x + (_p0.y - _p1.y) * _point.x + (_p1.x - _p0.x) * _point.y);

    return (s > 0 && t > 0 && (1 - s - t) > 0);
}

class Cube : public Shape
{
public:
    Cube(float _width, float _height, float _depth, glm::vec2 _position, float _z)
    {
        program = Shader("button");
        setupRender();

        m_position = _position;
        m_width = _width;
        m_height = _height;
        m_depth = _depth;
        m_z = _z;
    }

    bool m_active = false;
    bool pointCollision(glm::vec2 _point)
    {
        glm::mat4 pv = Camera::instance().cameraMatrix();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(m_position, m_z));
        model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(m_width, m_height, m_depth));
        glm::mat4 pvm = pv * model;

        m_points.clear();
        for (int i = 0; i < vertices.size(); i++)
        {
            glm::vec4 temp = pvm * vertices[i];
            m_points.push_back(glm::vec2(temp.x/temp.w, temp.y/temp.w));
        }

        

        for (int i = 0; i < m_points.size(); i += 3)
        {
            if (trianglePoint(glm::vec2(0.0f,0.0f), m_points[i], m_points[i + 1], m_points[i + 2]))
            {
                m_active = true;
                return true;
            }
        }
        m_active = false;
        return false;
    }

    void clickCube(bool _click)
    {
        if (_click)
        {
            colorStencil = { 0.0f,1.0f,0.0f };
        }
        else
        {
            colorStencil = { 1.0f,0.0f,0.0f };
        }
        
    }

    void setupRender()
    {
        unsigned int VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        std::string totalPath = ".//resources//textures//";
        totalPath.append("awesomeface.png");
        char const* path = totalPath.c_str();

        glGenTextures(1, &texture1);

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(true);
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

            glBindTexture(GL_TEXTURE_2D, texture1);
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

    void Render()
    {      
        program.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 pv = Camera::instance().cameraMatrix();

        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);

        program.setVec3("colorStencil", colorStencil);

        //normal object
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(m_position, m_z));
        model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(m_width, m_height, m_depth));
        glm::mat4 pvm = pv * model;
        program.setBool("stencil", false);
        program.setMat4("pvm", pvm);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);

        // large object
        if (m_active)
        {
            model = glm::mat4(1.0f);
            float scale = 1.01f;
            model = glm::translate(model, glm::vec3(m_position, m_z));
            model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(m_width * scale, m_height * scale, m_depth * scale));
            pvm = pv * model;
            program.setBool("stencil", true);
            program.setMat4("pvm", pvm);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }       

        glStencilMask(0xFF);
        glDisable(GL_STENCIL_TEST);

        

        

        glUseProgram(0);
    }

private:
    glm::vec3 colorStencil = { 1.0f,0.0f,0.0f };
    Shader program;
    unsigned int VAO, texture1;
    float m_width, m_height, m_depth, m_z;
    std::vector<glm::vec2> m_points;
};

class CubeFog
{
public:
    CubeFog(float _width, float _height, float _depth, glm::vec2 _position, float _z)
    {
        program = Shader("fog");
        setupRender();

        m_position = _position;
        m_width = _width;
        m_height = _height;
        m_depth = _depth;
        m_z = _z;
    }

    void setupRender()
    {
        unsigned int VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        std::string totalPath = ".//resources//textures//";
        totalPath.append("awesomeface.png");
        char const* path = totalPath.c_str();

        glGenTextures(1, &texture1);

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(true);
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

            glBindTexture(GL_TEXTURE_2D, texture1);
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

    void Render()
    {
        program.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 pv = Camera::instance().cameraMatrix();


        //normal object
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(m_position, m_z));
        model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(m_width, m_height, m_depth));
        glm::mat4 pvm = pv * model;
        glm::vec3 cameraPos = Camera::instance().Position;
        program.setVec3("cameraPos", cameraPos);
        program.setMat4("model", model);
        program.setMat4("pvm", pvm);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glUseProgram(0);
    }

private:
    glm::vec2 m_position;
    Shader program;
    unsigned int VAO, texture1;
    float m_width, m_height, m_depth, m_z;
    std::vector<glm::vec2> m_points;
};

class CubeGlass
{
public:
    CubeGlass(float _width, float _height, float _depth, glm::vec2 _position, float _z)
    {
        program = Shader("fog");
        setupRender();

        m_position = _position;
        m_width = _width;
        m_height = _height;
        m_depth = _depth;
        m_z = _z;
    }

    void setupRender()
    {
        unsigned int VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        std::string totalPath = ".//resources//textures//";
        totalPath.append("awesomeface.png");
        char const* path = totalPath.c_str();

        glGenTextures(1, &texture1);

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(true);
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

            glBindTexture(GL_TEXTURE_2D, texture1);
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

    void Render()
    {
        program.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 pv = Camera::instance().cameraMatrix();


        //normal object
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(m_position, m_z));
        model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(m_width, m_height, m_depth));
        glm::mat4 pvm = pv * model;
        glm::vec3 cameraPos = Camera::instance().Position;
        program.setVec3("cameraPos", cameraPos);
        program.setMat4("model", model);
        program.setMat4("pvm", pvm);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glUseProgram(0);
    }

private:
    glm::vec2 m_position;
    Shader program;
    unsigned int VAO, texture1;
    float m_width, m_height, m_depth, m_z;
    std::vector<glm::vec2> m_points;
};