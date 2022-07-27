#include "mesh.h"
#include <QFile>
#include <iostream>

Mesh::Mesh(std::string meshfile):
    m_meshfile(meshfile)
{
    loadFromObj();
}

Mesh::~Mesh()
{

}

struct int3 {
    int x;
    int y;
    int z;
};

// use member variable m_meshfile to try to load an .obj into a vao/vbo format
void Mesh::loadFromObj() {
    QFile file = QFile(QString::fromStdString(m_meshfile));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cout << "failed to open file: " << m_meshfile << std::endl;
    }

    //parse vertex and face lists
    std::vector<glm::vec3> vertexList;
    std::vector<int3> faceList;
    while (!file.atEnd()) {
        QString line = QString(file.readLine());
        QStringList tokens = line.split(" ");

        switch(tokens[0].toStdString().at(0)) {
        case 'v': // Vertex
            vertexList.push_back({tokens[1].toFloat(),
                                  tokens[2].toFloat(),
                                  tokens[3].toFloat()});
            break;
        case 'f': { // Face
            faceList.push_back({tokens[1].toInt(),
                                tokens[2].toInt(),
                                tokens[3].toInt()});
            break;
        }
        default:
            break;
        }
    }

    m_vertexData.clear();
    //process face lists into vertex data
    for (auto& face : faceList) {
        glm::vec3 v1 = vertexList.at(face.x-1);
        glm::vec3 v2 = vertexList.at(face.y-1);
        glm::vec3 v3 = vertexList.at(face.z-1);

        glm::vec3 hardNormal = glm::cross(v2 - v1, v3 - v1);

        insertVec3(m_vertexData,v1);
        insertVec3(m_vertexData,hardNormal);
        insertVec3(m_vertexData,v2);
        insertVec3(m_vertexData,hardNormal);
        insertVec3(m_vertexData,v3);
        insertVec3(m_vertexData,hardNormal);
    }
}
