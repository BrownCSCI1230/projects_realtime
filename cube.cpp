#include "cube.h"
#include <iostream>

Cube::Cube(int param1) :
    m_param1(param1)
{
    setVertexData();
}

Cube::~Cube()
{

}

void Cube::updateParams(int param1, int param2)
{
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft, glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, glm::vec3 topRight)
{
    // [TODO]: Task 2.1 -- create a tile (aka 2 triangles) out of the 4
    //         given points (once you have the tile, make sure you add
    //         the points to m_vertexData)
    // Note: pay attention to how you are calculating your normals! you will
    //       be using this function to create a cube

    // ======== TASK 2.1: TA SOLUTION ==========
    glm::vec3 t1Normal = glm::normalize(glm::cross(bottomLeft - topLeft,
                                                   bottomRight - bottomLeft));
    glm::vec3 t2Normal = glm::normalize(glm::cross(topRight - bottomRight,
                                                   topLeft - topRight));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, t1Normal);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, t1Normal);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, t1Normal);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, t2Normal);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, t2Normal);
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, t2Normal);
}

void Cube::makeFace(glm::vec3 topLeft, glm::vec3 bottomLeft,
                    glm::vec3 bottomRight, glm::vec3 topRight)
{
    // [TODO]: Task 2.2 -- create a single side of the cube out of the 4
    //         given points and makeTile()
    // Note: think about how param 1 affects the number of triangles on
    //       the face of the cube

    // ======== TASK 2.2: TA SOLUTION ==========
    glm::vec3 colChange((topRight.x - topLeft.x) / m_param1,
                        (topRight.y - topLeft.y) / m_param1,
                        (topRight.z - topLeft.z) / m_param1);
    glm::vec3 rowChange((bottomRight.x - topRight.x) / m_param1,
                        (bottomRight.y - topRight.y) / m_param1,
                        (bottomRight.z - topRight.z) / m_param1);

    for (int row = 0; row < m_param1; row++) {
        for (int col = 0; col < m_param1; col++) {

            // top left
            glm::vec3 v1 = glm::vec3(topLeft.x + (col*colChange.x) + (row*rowChange.x),
                                     topLeft.y + (col*colChange.y) + (row*rowChange.y),
                                     topLeft.z + (col*colChange.z) + (row*rowChange.z));
            // bottom left
            glm::vec3 v2 = v1 + rowChange;
            // bottom right
            glm::vec3 v3 = v1 + rowChange + colChange;
            // top right
            glm::vec3 v4 = v1 + colChange;

            makeTile(v1, v2, v3, v4);
        }
    }
}

void Cube::setVertexData()
{
    // uncomment this for task 2.2:
//    makeTile(glm::vec3(-0.5f, 0.5f, 0.5f),
//             glm::vec3(-0.5f, -0.5f, 0.5f),
//             glm::vec3(0.5f, -0.5f, 0.5f),
//             glm::vec3(0.5f, 0.5f, 0.5f));


    // [TODO]: Task 2.3 -- Use the makeFace() function to make all 6 sides of the cube

    // ======== TASK 2.3: TA SOLUTION ==========
    // positive z
    makeFace(glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(0.5f, 0.5f, 0.5f));
    // negative z
    makeFace(glm::vec3(0.5f, 0.5f,-0.5f),
             glm::vec3(0.5f,-0.5f,-0.5f),
             glm::vec3(-0.5f,-0.5f,-0.5f),
             glm::vec3(-0.5f, 0.5f,-0.5f));

    // positive x
    makeFace(glm::vec3(0.5f, 0.5f, 0.5f),
             glm::vec3(0.5f,-0.5f, 0.5f),
             glm::vec3(0.5f,-0.5f,-0.5f),
             glm::vec3(0.5f, 0.5f,-0.5f));

    // negative x
    makeFace(glm::vec3(-0.5f, 0.5f,-0.5f),
             glm::vec3(-0.5f,-0.5f,-0.5f),
             glm::vec3(-0.5f,-0.5f, 0.5f),
             glm::vec3(-0.5f, 0.5f, 0.5f));

    // positive y
    makeFace(glm::vec3(0.5f, 0.5f,-0.5f),
             glm::vec3(-0.5f, 0.5f,-0.5f),
             glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(0.5f, 0.5f, 0.5f));

    // negative y
    makeFace(glm::vec3(0.5f,-0.5f, 0.5f),
             glm::vec3(-0.5f,-0.5f, 0.5f),
             glm::vec3(-0.5f,-0.5f,-0.5f),
             glm::vec3(0.5f,-0.5f,-0.5f));
}

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
void Cube::insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

std::vector<float> Cube::generateShape()
{
    return m_vertexData;
}
