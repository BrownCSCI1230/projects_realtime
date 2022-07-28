#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H

#include <vector>
#include <glm/glm.hpp>

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
inline void insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

inline void insertVec2(std::vector<float> &data, glm::vec2 v){
    data.push_back(v.x);
    data.push_back(v.y);
}


class OpenGLShape
{
public:
    OpenGLShape();
    ~OpenGLShape();

    std::vector<float> generateShape();
    virtual void updateParams(int param1, int param2);

protected:
    std::vector<float> m_vertexData;
};

#endif // OPENGLSHAPE_H
