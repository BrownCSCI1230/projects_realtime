#ifndef VAO_H
#define VAO_H

#include "vbo.h"
#include "shader.h"
#include <memory>

enum VAOType {
    POS_NORM,
    POS_UV,
    POS_NORM_UV
};

class vao
{
public:
    vao(std::shared_ptr<vbo> vbo, VAOType type);
    void finish();

    void bind();
    void unbind();
    void addAttribute(int attribute_size, int vert_size);
    void draw(Shader shader);

private:
    std::shared_ptr<vbo> m_vbo;
    GLuint m_handle;
    GLint m_curr_offset;
    GLuint m_num_attribs;
};

#endif // VAO_H
