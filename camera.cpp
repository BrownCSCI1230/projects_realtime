#include "camera.h"
#include <iostream>

Camera::Camera()
{

}

void Camera::initialize(CS123::CS123SceneMetaData data, int w, int h){
    m_pos = glm::vec3(data.cameraData.pos);
    m_look = glm::vec3(data.cameraData.look);
    m_up = glm::vec3(data.cameraData.up);


    setView();
}

Camera::~Camera()
{

}

void Camera::finish()
{

}

void Camera::setView()
{
    m_view = glm::lookAt(m_pos, m_pos+m_look, m_up);
}
glm::mat4 Camera::getView(){
    return m_view;
}
