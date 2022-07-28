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

    glm::mat4 getView();

private:
    void setView();

    glm::vec3 m_pos;
    glm::vec3 m_look;
    glm::vec3 m_up;

    glm::mat4 m_view;
};

#endif // CAMERA_H
