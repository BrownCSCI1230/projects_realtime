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

void Camera::onKeyPressed(QKeyEvent *event){
    glm::vec3 look = glm::normalize(m_look);
    glm::vec3 perp = glm::normalize(glm::vec3(look.z, 0, -look.x));
    if(event->key() == Qt::Key_W){
        m_pos += look;
    }
    if(event->key() == Qt::Key_S){
        m_pos -= look;
    }
    if(event->key() == Qt::Key_A){
        m_pos += perp;
    }
    if(event->key() == Qt::Key_D){
        m_pos -= perp;
    }
    if(event->key() == Qt::Key_Space){
        m_pos += m_up;
    }
    if(event->key() == Qt::Key_Control){
        m_pos -= m_up;
    }
    if(event->key() == Qt::Key_E){
        std::cout << "befor: " << m_fov << std::endl;
        m_fov++;
        std::cout << "after: " << m_fov << std::endl;
        setProjection();
    }
    setView();
}

void Camera::mouseMoved(float x, float y){
    glm::vec3 look = glm::normalize(m_look);
    glm::vec3 perp = glm::normalize(glm::vec3(look.z, 0, -look.x));

    glm::mat4 xrot = glm::mat4(1);
    glm::mat4 yrot = glm::mat4(1);

    xrot = glm::rotate(xrot, -x/500.f, glm::vec3(0.0f, 1.0f, 0.0f));
    yrot = glm::rotate(yrot, y/500.f, perp);

    xrot = rotate(-x/500.f, glm::vec3(0.0f, 1.0f, 0.0f));
    yrot = rotate(y/500.f, perp);

    m_look = xrot*yrot*glm::vec4(m_look, 0.f);

    setView();
}

glm::mat4 Camera::rotate(float angle, glm::vec3 axis){
    float a = angle;
    float c = glm::cos(a);
    float s = glm::sin(a);
    glm::vec3 u = glm::normalize(axis);

    glm::mat3 rot;
    rot[0][0] = c+u.x*u.x*(1.f-c);
    rot[0][1] = u.x*u.y*(1.f-c)-u.z*s;
    rot[0][2] = u.x*u.z*(1.f-c)+u.y*s;

    rot[1][0] = u.x*u.y*(1.f-c)+u.z*s;
    rot[1][1] = c+u.y*u.y*(1.f-c);
    rot[1][2] = u.y*u.z*(1.f-c)-u.x*s;

    rot[2][0] = u.x*u.z*(1.f-c)-u.y*s;
    rot[2][1] = u.y*u.z*(1.f-c)+u.x*s;
    rot[2][2] = c+u.z*u.z*(1.f-c);

    return glm::mat4(rot);
}
