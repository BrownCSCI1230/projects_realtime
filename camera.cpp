#include "camera.h"
#include <iostream>

Camera::Camera()
{

}

void Camera::initialize(CS123::CS123SceneMetaData data, int w, int h){
    m_near = 0.1f;
    m_far = 100.f;
    m_fov = data.cameraData.heightAngle;
    m_pos = glm::vec3(data.cameraData.pos);
    m_look = glm::vec3(data.cameraData.look);
    m_up = glm::vec3(data.cameraData.up);
    m_aspect = w/(float)h;


    setProjection();
    setView();
}

Camera::~Camera()
{

}

void Camera::finish()
{

}

void Camera::setProjection(){
    m_proj = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

glm::mat4 Camera::getProjection(){
    return m_proj;
}

void Camera::setView()
{
    m_view = glm::lookAt(m_pos, m_pos+m_look, m_up);
}
glm::mat4 Camera::getView(){
    return m_view;
}

void Camera::resize(int w, int h){
    m_aspect = w/(float)h;
    setProjection();
}
