#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <glm/glm.hpp>

class Cube
{
public:
    Cube(int param1);
    ~Cube();

    void updateParams(int param1, int param2);
    std::vector<float> generateShape();

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();
    void makeTile(glm::vec3 topLeft, glm::vec3 bottomLeft,
                  glm::vec3 bottomRight, glm::vec3 topRight);
    void makeFace(glm::vec3 topLeft, glm::vec3 bottomLeft,
                  glm::vec3 bottomRight, glm::vec3 topRight);

    std::vector<float> m_vertexData;
    int m_param1;
};

#endif // CUBE_H
