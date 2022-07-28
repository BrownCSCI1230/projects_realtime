#include "texture.h"
#include <QImage>
#include <QString>

bool Texture::initialize(std::string filePath, int texUnit) {
    m_texture_path = filePath;

    GLenum texUnitEnum = GL_TEXTURE0 + texUnit;
    if (texUnitEnum >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        return false;
    }

    m_texture_unit.as_int = texUnit;
    m_texture_unit.as_enum = texUnitEnum;

    QImage img;
    if (!img.load(QString::fromStdString(filePath))) {
        return false;
    }

    m_texture_type = GL_TEXTURE_2D;

    glGenTextures(1, &m_handle);
    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,img.width(),img.height(),0,GL_RGBA,GL_UNSIGNED_BYTE,img.bits());
    glGenerateMipmap(GL_TEXTURE_2D);

    unbind();

    return true;
}

int Texture::getTextureUnit(){
    return m_texture_unit.as_int;
}

void Texture::finish() {
    glDeleteTextures(1, &m_handle);
}

void Texture::bind() {
    glActiveTexture(m_texture_unit.as_enum);
    glBindTexture(m_texture_type,m_handle);
}

void Texture::unbind() {
    glBindTexture(m_texture_type,0);
    glActiveTexture(GL_TEXTURE0);

}
