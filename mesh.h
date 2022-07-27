#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "openglshape.h"

class Mesh : public OpenGLShape
{
public:
    Mesh(std::string meshfile);
    ~Mesh();

private:
    void loadFromObj();

    std::string m_meshfile;
};

#endif // MESH_H
