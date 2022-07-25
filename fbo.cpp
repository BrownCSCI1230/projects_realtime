#include "fbo.h"

fbo::fbo()
{

}

void fbo::initialize(int w, int h)
{
    m_width = w;
    m_height = h;

    //Generate Texture
    glGenTextures(1, &m_fbo_texture);
    bindTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unbindTexture();

    //Make a framebuffer and use your texture as a color attachment
    glGenFramebuffers(1, &m_fbo);
    bindFBO();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fbo_texture, 0);

    //Renderbuffer for depth and stencil attachments
    glGenRenderbuffers(1, &m_rbo);
    bindRBO();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    //Unbinding
    unbindRBO();
    unbindFBO();
}

fbo::~fbo()
{

}

void fbo::finish()
{
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteFramebuffers(1, &m_fbo);
    glDeleteTextures(1, &m_fbo_texture);
}

void fbo::bindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void fbo::unbindFBO()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void fbo::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);
}

void fbo::unbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void fbo::bindRBO()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
}

void fbo::unbindRBO()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
