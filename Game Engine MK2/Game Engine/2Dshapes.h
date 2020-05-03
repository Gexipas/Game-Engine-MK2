#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class Shape
{
public:
    virtual bool pointCollision(glm::vec2 _point) = 0;
    glm::vec2 m_position;
};

class Rect : public Shape
{
public:
    Rect(int _width, int _height, glm::vec2 _position)
    {
        m_position = _position;
        m_width = _width;
        m_height = _height;
    }

    bool pointCollision(glm::vec2 _point)
    {
        if ((_point.x > m_position.x - (float)(m_width / 2)) &&
            (_point.x < m_position.x + (float)(m_width / 2)) &&
            (_point.y > m_position.y - (float)(m_height / 2)) &&
            (_point.y < m_position.y + (float)(m_height / 2)))
        {
            return true;
        }
        return false;

    }
private:
    int m_width, m_height;
};

class Circle : public Shape
{
public:
    Circle(int _radius, glm::vec2 _position)
    {
        m_position = _position;
        m_radius = _radius;
    }

    bool pointCollision(glm::vec2 _point)
    {
        if (glm::length(m_position - _point) < m_radius)
        {
            return true;
        }
        return false;
    }
private:
    int m_radius;
};