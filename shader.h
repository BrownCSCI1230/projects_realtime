#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"
#include <string>
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Shader
{
public:
    Shader();
    void initialize(GLuint id);
    ~Shader();
    void finish();

    bool setUniform1f(std::string varName, float value);
    bool setUniform1i(std::string varName, int value);
    bool setUniformVec3(std::string varName, glm::vec3 value);
    bool setUniformVec4(std::string varName, glm::vec4 value);
    bool setUniformMat4(std::string varName, glm::mat4 value);
    void bind();
    void unbind();

private:
    GLuint m_id;
};

#endif // SHADER_H
