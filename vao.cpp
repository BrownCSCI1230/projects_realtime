#include "vao.h"
#include <iostream>

vao::vao(std::shared_ptr<vbo> vbo, VAOType type)
{
    m_vbo = vbo;
    m_curr_offset = 0;
    m_num_attribs = 0;

    glGenVertexArrays(1, &m_handle);

    switch(type) {
    case VAOType::POS_NORM:
        addAttribute(3, 6); //Add positions
        addAttribute(3, 6); //Add normals
        break;
    case VAOType::POS_UV:
        addAttribute(3, 5); //Add positions
        addAttribute(2, 5); //Add uvs
        break;
    case VAOType::POS_NORM_UV:
        addAttribute(3, 8); //Add positions
        addAttribute(3, 8); //Add normals
        addAttribute(2, 8); //Add uvs
        break;
    default:
        break;
    }
}

void vao::finish()
{
    glDeleteVertexArrays(1, &m_handle);
}

void vao::bind()
{
    glBindVertexArray(m_handle);
}

void vao::unbind()
{
    glBindVertexArray(0);
}

void vao::addAttribute(int attribute_size, int vert_size)
{
    bind();
    m_vbo->bind();
    glEnableVertexAttribArray(m_num_attribs);
    glVertexAttribPointer(m_num_attribs, attribute_size, GL_FLOAT, GL_FALSE, vert_size*sizeof(GL_FLOAT), reinterpret_cast<void *>(m_curr_offset * sizeof(GLfloat)));
    m_vbo->unbind();
    unbind();
    m_curr_offset += attribute_size;
    m_num_attribs += 1;
}

void vao::draw(Shader shader)
{
    shader.bind();
    bind();
    glDrawArrays(GL_TRIANGLES, 0, m_vbo->getLength()/m_curr_offset);
    shader.unbind();
}
