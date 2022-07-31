#include "cylinder.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"

Cylinder::Cylinder(int param1, int param2):
    m_param1(param1),
    m_param2(param2)
{
    setVertexData();
}

void Cylinder::updateParams(int param1, int param2)
{
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

Cylinder::~Cylinder() {
}

glm::vec3 Cylinder::cylToCartesian(float theta, float rho, float height) {
    return glm::vec3(glm::cos(theta) * rho,
                     height,
                     glm::sin(theta) * rho);
}

void Cylinder::setVertexData() {
    // iterate through theta slices
    // iterate through radial slices
    for(int iTheta = 0; iTheta < m_param1; iTheta++) {
        for(int iRho = 0; iRho < m_param2; iRho++) {
            float the1 = 1.0 * iTheta / m_param1 * 2 * glm::pi<float>();
            float the2 = 1.0 * (iTheta + 1) / m_param1 * 2 * glm::pi<float>();

            float rho1 = 0.5 * iRho / m_param2;
            float rho2 = 0.5 * (iRho + 1) / m_param2;

            float height1 = 0.5 - 2 * rho1;
            float height2 = 0.5 - 2 * rho2;

            glm::vec3 p1 = cylToCartesian(the1, rho1, height1);
            glm::vec3 p2 = cylToCartesian(the1, rho2, height2);
            glm::vec3 p3 = cylToCartesian(the2, rho2, height2);
            glm::vec3 p4 = cylToCartesian(the2, rho1, height1);

            glm::vec3 d1 = glm::normalize(p2 - p1);
            glm::vec3 d2 = glm::normalize(p3 - p4);

            p1.y = -0.5;
            p2.y = -0.5;
            p3.y = -0.5;
            p4.y = -0.5;

            glm::vec3 botNorm = glm::vec3(0,-1,0);

            if (iRho == 0) {
                insertVec3(m_vertexData,p1);
                insertVec3(m_vertexData,botNorm);

                insertVec3(m_vertexData,p2);
                insertVec3(m_vertexData,botNorm);

                insertVec3(m_vertexData,p3);
                insertVec3(m_vertexData,botNorm);

            } else {
                insertVec3(m_vertexData,p1);
                insertVec3(m_vertexData,botNorm);

                insertVec3(m_vertexData,p2);
                insertVec3(m_vertexData,botNorm);

                insertVec3(m_vertexData,p3);
                insertVec3(m_vertexData,botNorm);

                insertVec3(m_vertexData,p1);
                insertVec3(m_vertexData,botNorm);

                insertVec3(m_vertexData,p3);
                insertVec3(m_vertexData,botNorm);

                insertVec3(m_vertexData,p4);
                insertVec3(m_vertexData,botNorm);
            }

            p1.y = 0.5;
            p2.y = 0.5;
            p3.y = 0.5;
            p4.y = 0.5;

            glm::vec3 topNorm = glm::vec3(0,1,0);

            if (iRho == 0) {
                insertVec3(m_vertexData,p3);
                insertVec3(m_vertexData,topNorm);

                insertVec3(m_vertexData,p2);
                insertVec3(m_vertexData,topNorm);

                insertVec3(m_vertexData,p1);
                insertVec3(m_vertexData,topNorm);

            } else {
                insertVec3(m_vertexData,p3);
                insertVec3(m_vertexData,topNorm);

                insertVec3(m_vertexData,p2);
                insertVec3(m_vertexData,topNorm);

                insertVec3(m_vertexData,p1);
                insertVec3(m_vertexData,topNorm);

                insertVec3(m_vertexData,p4);
                insertVec3(m_vertexData,topNorm);

                insertVec3(m_vertexData,p3);
                insertVec3(m_vertexData,topNorm);

                insertVec3(m_vertexData,p1);
                insertVec3(m_vertexData,topNorm);
            }

            p1 = cylToCartesian(the1, 0.5, height1);
            p2 = cylToCartesian(the1, 0.5, height2);
            p3 = cylToCartesian(the2, 0.5, height2);
            p4 = cylToCartesian(the2, 0.5, height1);
            glm::vec3 norm1 = glm::normalize(glm::vec3(p1.x,0,p1.z));
            glm::vec3 norm2 = glm::normalize(glm::vec3(p3.x,0,p3.z));

            insertVec3(m_vertexData,p3);
            insertVec3(m_vertexData,norm2);

            insertVec3(m_vertexData,p2);
            insertVec3(m_vertexData,norm1);

            insertVec3(m_vertexData,p1);
            insertVec3(m_vertexData,norm1);

            insertVec3(m_vertexData,p4);
            insertVec3(m_vertexData,norm2);

            insertVec3(m_vertexData,p3);
            insertVec3(m_vertexData,norm2);

            insertVec3(m_vertexData,p1);
            insertVec3(m_vertexData,norm1);
        }
    }
}

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

std::vector<float> Cylinder::generateShape()
{
    return m_vertexData;
}
