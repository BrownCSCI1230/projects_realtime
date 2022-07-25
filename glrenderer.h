#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "GL/glew.h"
#include <QOpenGLWidget>
#include "glm/glm.hpp"
#include "vbo.h"
#include "vao.h"
#include "fbo.h"
#include "Parsing/cs123sceneloader.h"
#include "ShaderLoading/shaderloader.h"
#include "camera.h"
#include "shader.h"

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
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    Shader m_shader;    //Stores phong program
    Shader m_fboShader; //Stores postprocessing program

    fbo m_fbo; //Stores fbo
    std::shared_ptr<vbo> m_fullscreen_vbo;  //Stores vbo for fullscreen quad
    std::shared_ptr<vao> m_fullscreen_vao;  //Stores vao for fullscreen quad

    Camera m_cam;  //Stores camera object
    std::vector<std::shared_ptr<vao>> m_vaos;   //Stores vaos for each shape (4 total)
    CS123::CS123SceneMetaData m_metaData;   //Stores scene data
    glm::vec2 m_prev_mouse_pos; //Stores mouse position
};

#endif // GLRENDERER_H
