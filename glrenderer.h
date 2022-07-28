#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "GL/glew.h"
#include <QOpenGLWidget>
#include "glm/glm.hpp"
#include "camera.h"
#include "Parsing/cs123sceneloader.h"
#include "ShaderLoading/shaderloader.h"

class GLRenderer : public QOpenGLWidget
{
public:
    GLRenderer(QWidget *parent = nullptr);
    void finish();
    void settingsChange();

protected:
    void initializeGL() override;   //Called once at the start of the program
    void paintGL() override;    //Called every frame in a loop
    void resizeGL(int width, int height) override;  //Called when window size changes

private:
};

#endif // GLRENDERER_H
