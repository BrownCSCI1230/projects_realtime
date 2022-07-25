#include "vbo.h"
#include <iostream>

vbo::vbo(std::vector<float> data)
{
    m_length = data.size();
    glGenBuffers(1, &m_handle);
    bind();
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float), data.data(), GL_STATIC_DRAW);
    unbind();
}

void vbo::finish()
{
    glDeleteBuffers(1, &m_handle);
}

void vbo::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
}

void vbo::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLsizei vbo::getLength(){
    return m_length;
}
