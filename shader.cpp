#include "shader.h"

Shader::Shader()
{

}

void Shader::initialize(GLuint id)
{
    m_id = id;
}

Shader::~Shader(){

}

void Shader::finish()
{
    glDeleteProgram(m_id);
}

bool Shader::setUniform1f(std::string varName, float value){
    bind();
    int varLoc = glGetUniformLocation(m_id, varName.c_str());
    if(varLoc == -1){
        unbind();
        return false;
    }
    glUniform1f(varLoc, value);
    unbind();
    return true;
}

bool Shader::setUniform1i(std::string varName, int value){
    bind();
    int varLoc = glGetUniformLocation(m_id, varName.c_str());
    if(varLoc == -1){
        unbind();
        return false;
    }
    glUniform1i(varLoc, value);
    unbind();
    return true;
}

bool Shader::setUniformVec3(std::string varName, glm::vec3 value){
    bind();
    int varLoc = glGetUniformLocation(m_id, varName.c_str());
    if(varLoc == -1){
        unbind();
        return false;
    }
    glUniform3f(varLoc, value.x, value.y, value.z);
    unbind();
    return true;
}

bool Shader::setUniformVec4(std::string varName, glm::vec4 value){
    bind();
    int varLoc = glGetUniformLocation(m_id, varName.c_str());
    if(varLoc == -1){
        unbind();
        return false;
    }
    glUniform4f(varLoc, value.x, value.y, value.z, value.w);
    unbind();
    return true;
}

bool Shader::setUniformMat4(std::string varName, glm::mat4 value){
    bind();
    int varLoc = glGetUniformLocation(m_id, varName.c_str());
    if(varLoc == -1){
        unbind();
        return false;
    }
    glUniformMatrix4fv(varLoc, 1, GL_FALSE, glm::value_ptr(value));
    unbind();
    return true;
}

void Shader::bind(){
    glUseProgram(m_id);
}

void Shader::unbind(){
    glUseProgram(0);
}
