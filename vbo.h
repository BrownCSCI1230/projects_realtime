#ifndef VBO_H
#define VBO_H

#include "GL/glew.h"
#include <vector>

class vbo
{
public:
    vbo(std::vector<float> data);
    void finish();

    void bind();
    void unbind();
    GLsizei getLength();

private:
    GLuint m_handle;
    GLsizei m_length;
};

#endif // VBO_H
