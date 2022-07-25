#ifndef CAMERA_H
#define CAMERA_H

#include "Parsing/cs123sceneloader.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include <QKeyEvent>

class Camera
{
public:
    Camera();
    void initialize(CS123::CS123SceneMetaData data, int w, int h);
    ~Camera();
    void finish();

    glm::mat4 getProjection();
    glm::mat4 getView();
    void resize(int w, int h);
    void onKeyPressed(QKeyEvent *event);
    void mouseMoved(float x, float y);
    glm::mat4 rotate(float angle, glm::vec3 axis);

private:
    void setView();
    void setProjection();

    float m_near;
    float m_far;
    float m_aspect;
    float m_fov;

    glm::vec3 m_pos;
    glm::vec3 m_look;
    glm::vec3 m_up;

    glm::mat4 m_proj;
    glm::mat4 m_view;
};

#endif // CAMERA_H
