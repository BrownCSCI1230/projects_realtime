#ifndef FBO_H
#define FBO_H

#include "GL/glew.h"
#include <iostream>

class fbo
{
public:
    fbo();
    void initialize(int w, int h);
    ~fbo();
    void finish();

    void bindFBO();
    void unbindFBO();
    void bindTexture();
    void unbindTexture();
    void bindRBO();
    void unbindRBO();

private:
    int m_width;
    int m_height;
    GLuint m_fbo_texture;
    GLuint m_fbo;
    GLuint m_rbo;
};

#endif // FBO_H
