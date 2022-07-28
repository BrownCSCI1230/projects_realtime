#include "glrenderer.h"

#include <QCoreApplication>
#include <set>
#include "settings.h"


GLRenderer::GLRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void GLRenderer::finish()
{
    makeCurrent();
    doneCurrent();
}

void GLRenderer::initializeGL()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));
    //-------------------------------------------------------------//

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    std::cout<<"End of initializeGL"<<std::endl;
}

void GLRenderer::paintGL()
{
    std::cout<<"start of paintGl"<<std::endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Allow them to use this one call here? It is the only one that needs to be only called once per loop I think
}

void GLRenderer::resizeGL(int w, int h)
{
    update();
}

void GLRenderer::settingsChange(){
    update();
}
