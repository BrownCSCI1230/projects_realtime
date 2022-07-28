#include "sphere.h"
#include "ext/scalar_constants.hpp"
#include <iostream>

Sphere::Sphere(int param1, int param2) :
    m_radius(0.5),
    m_param1(param1),
    m_param2(param2)
{
    setVertexData();
}

Sphere::~Sphere()
{

}

void Sphere::updateParams(int param1, int param2)
{
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

float thetaToU(float theta) {
    return theta/(2.0f * glm::pi<float>());
}

float phiToV(float phi) {
    return 1 - phi/glm::pi<float>();
}

void Sphere::makeTile(glm::vec3 topLeft, glm::vec3 bottomLeft,
                      glm::vec3 bottomRight, glm::vec3 topRight, glm::vec4 uvinfo) {
    float uleft = thetaToU(uvinfo.x);
    float uright = thetaToU(uvinfo.y);
    float vtop = phiToV(uvinfo.z);
    float vbot = phiToV(uvinfo.w);

    glm::vec3 tlNormal = glm::normalize(topLeft);
    glm::vec3 blNormal = glm::normalize(bottomLeft);
    glm::vec3 brNormal = glm::normalize(bottomRight);
    glm::vec3 trNormal = glm::normalize(topRight);

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, tlNormal);
    insertVec2(m_vertexData, glm::vec2(uleft,vtop));

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, blNormal);
    insertVec2(m_vertexData, glm::vec2(uleft,vbot));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, brNormal);
    insertVec2(m_vertexData, glm::vec2(uright,vbot));

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, brNormal);
    insertVec2(m_vertexData, glm::vec2(uright,vbot));

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, trNormal);
    insertVec2(m_vertexData, glm::vec2(uright,vtop));

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, tlNormal);
    insertVec2(m_vertexData, glm::vec2(uleft,vtop));
}

void Sphere::makeSlice(float currentTheta, float nextTheta) {

    // [TODO]: Task 3.1.2 -- create a single slice of the sphere using the
    //         makeTile() function you implemented in Task 3.1.1
    // Note: think about how param 1 comes into play here!

    // ======== TASK 3.1.2: TA SOLUTION ==========
    float phiStep = glm::radians(180.f / m_param1);
    for (int bite = 0; bite < m_param1; bite++) {
        float bottomPhi = (bite % (m_param1 + 1)) * phiStep;
        float topPhi = ((bite + 1) % (m_param1 + 1)) * phiStep;

        // top left vertex
        glm::vec3 v1(m_radius * std::sin(topPhi) * std::cos(currentTheta),
                     m_radius * std::cos(topPhi),
                     m_radius * std::sin(topPhi) * std::sin(currentTheta));

        // bottom left vertex
        glm::vec3 v2(m_radius * std::sin(bottomPhi) * std::cos(currentTheta),
                     m_radius * std::cos(bottomPhi),
                     m_radius * std::sin(bottomPhi) * std::sin(currentTheta));

        // bottom right vertex
        glm::vec3 v3(m_radius * std::sin(bottomPhi) * std::cos(nextTheta),
                     m_radius * std::cos(bottomPhi),
                     m_radius * std::sin(bottomPhi) * std::sin(nextTheta));

        // top right vertex
        glm::vec3 v4(m_radius * std::sin(topPhi) * std::cos(nextTheta),
                     m_radius * std::cos(topPhi),
                     m_radius * std::sin(topPhi) * std::sin(nextTheta));

        makeTile(v1, v2, v3, v4, glm::vec4(currentTheta, nextTheta, topPhi, bottomPhi));
    }
}

void Sphere::makeSphere()
{
    // [TODO]: Task 3.2 -- create a full sphere using the makeSlice() function you
    //         implemented in Task 3.1.2
    // Note: think about how param 2 comes into play here!

    // ======== TASK 3.2: TA SOLUTION ==========
    float thetaStep = glm::radians(360.f / m_param2);
    for (int slice = 0; slice < m_param2; slice++) {
        float currentTheta = slice * thetaStep;
        float nextTheta = (slice + 1) * thetaStep;
        makeSlice(currentTheta, nextTheta);
    }
}

void Sphere::setVertexData() {
    // Uncomment to make slice
//    float thetaStep = glm::radians(360.f / m_param2);
//    float currentTheta = 0 * thetaStep;
//    float nextTheta = 1 * thetaStep;
//    makeSlice(currentTheta, nextTheta);

    // Uncomment to make sphere
    makeSphere();
}
