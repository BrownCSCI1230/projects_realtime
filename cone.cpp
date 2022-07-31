#include "cone.h"
#include "glm/gtc/constants.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"


Cone::Cone(int param1, int param2) :
    m_param1(param1),
    m_param2(param2)
{
    setVertexData();
}

void Cone::updateParams(int param1, int param2)
{
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

Cone::~Cone()
{

}

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

glm::vec3 Cone::cylToCartesian(float theta, float rho, float height) {
    return glm::vec3(glm::cos(theta) * rho,
                     height,
                     glm::sin(theta) * rho);
}

void Cone::setVertexData() {
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

            glm::vec3 norm1 = glm::normalize(glm::vec3(d1.x,-d1.y,d1.z));
            glm::vec3 norm2 = glm::normalize(glm::vec3(d2.x,-d2.y,d2.z));



            norm1 = glm::vec3(glm::rotate(90.0f,glm::cross(d1, norm1)) * glm::vec4(d1,0));
            norm2 = glm::vec3(glm::rotate(90.0f,glm::cross(d2, norm2)) * glm::vec4(d2,0));


            if (iRho == 0) {
                insertVec3(m_vertexData,p3);
                insertVec3(m_vertexData,norm2);

                insertVec3(m_vertexData,p2);
                insertVec3(m_vertexData,norm1);

                insertVec3(m_vertexData,p1);
                insertVec3(m_vertexData,glm::normalize(norm1 + norm2));

            } else {
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
        }
    }
}

std::vector<float> Cone::generateShape()
{
    return m_vertexData;
}
