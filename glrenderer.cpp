#include "glrenderer.h"

#include <QCoreApplication>
#include <set>
#include "sphere.h"
#include "cube.h"
#include "settings.h"

const uint SPHERE_VAO_INDEX = 0;
const uint CUBE_VAO_INDEX = 1;
const uint CONE_VAO_INDEX = 2;
const uint CYLINDER_VAO_INDEX = 3;

GLRenderer::GLRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void GLRenderer::finish()
{
    m_shader.finish();
    m_cam.finish();
    makeCurrent();
    doneCurrent();
}

void GLRenderer::initializeGL()
{
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    fprintf(stdout, "Using GLEW %s\n", glewGetString(GLEW_VERSION));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    m_shader.initialize(ShaderLoader::createShaderProgram("Resources/Shaders/phong.vert", "Resources/Shaders/phong.frag"));
    std::cout<<"FBO initialized"<<std::endl;

    //Load Scene Data
    CS123::CS123SceneLoader::load("Resources/SceneFiles/ballspec.xml", m_metaData);

    //Initialize Primitive VAOs

    //Add Sphere data to vao vector
    Sphere sph = Sphere(30, 30);
    std::shared_ptr<vbo> sphere_vbo = std::make_shared<vbo>(sph.generateShape());
    std::shared_ptr<vao> sphere_vao = std::make_shared<vao>(sphere_vbo, VAOType::POS_NORM);
    m_vaos.push_back(sphere_vao);

    //Add Cube data to vao vector
    Cube cube = Cube(2);
    std::shared_ptr<vbo> cube_vbo = std::make_shared<vbo>(cube.generateShape());
    std::shared_ptr<vao> cube_vao = std::make_shared<vao>(cube_vbo, VAOType::POS_NORM);
    m_vaos.push_back(cube_vao);

    //Set camera data
    m_cam.initialize(m_metaData, size().width(), size().height());
    std::cout<<"End of initializeGL"<<std::endl;
}

void GLRenderer::paintGL()
{
    std::cout<<"paintgl"<<std::endl;
    //m_fbo.bindFBO();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Allow them to use this one call here? It is the only one that needs to be only called once per loop I think

    glm::mat4 view = m_cam.getView();
    //Set Camera Uniforms
    m_shader.setUniformMat4("view", view);
    m_shader.setUniformMat4("projection", m_cam.getProjection());

    //Set Global Uniforms
    m_shader.setUniform1f("ka", m_metaData.globalData.ka);
    m_shader.setUniform1f("kd", m_metaData.globalData.kd);
    m_shader.setUniform1f("ks", m_metaData.globalData.ks);

    //Set Lights
    m_shader.setUniform1i("numLights", m_metaData.lights.size());
    for(int j = 0; j<m_metaData.lights.size(); j++){
        if(m_metaData.lights[j].type == LightType::LIGHT_DIRECTIONAL){
            m_shader.setUniform1i("lightType["+std::to_string(j)+"]", 1);
            m_shader.setUniformVec4("camSpace_lightDir["+std::to_string(j)+"]", view * m_metaData.lights[j].dir);
            m_shader.setUniformVec4("lightColor["+std::to_string(j)+"]", m_metaData.lights[j].color);
        }
    }

    //Set Shapes
    for(int i = 0; i < m_metaData.shapes.size(); i++){
        CS123::CS123SceneShapeData& shape = m_metaData.shapes[i];
        m_shader.setUniformMat4("model", shape.ctm);
        m_shader.setUniformVec4("obj_ambient_color", shape.primitive.material.cAmbient);
        m_shader.setUniformVec4("obj_diffuse_color", shape.primitive.material.cDiffuse);
        m_shader.setUniformVec4("obj_specular_color", shape.primitive.material.cSpecular);
        m_shader.setUniform1f("shininess", shape.primitive.material.shininess);

        switch (shape.primitive.type)  {
            case PrimitiveType::PRIMITIVE_CUBE:
                m_vaos[CUBE_VAO_INDEX]->draw(m_shader);
                break;
            case PrimitiveType::PRIMITIVE_SPHERE:
                m_vaos[SPHERE_VAO_INDEX]->draw(m_shader);
                break;
            case PrimitiveType::PRIMITIVE_CONE:
                m_vaos[CONE_VAO_INDEX]->draw(m_shader);
                break;
            case PrimitiveType::PRIMITIVE_CYLINDER:
                m_vaos[CYLINDER_VAO_INDEX]->draw(m_shader);
                break;
            default:
                break;
        }
    }
}

void GLRenderer::resizeGL(int w, int h)
{
    m_cam.resize(w, h);
    update();
}

void GLRenderer::settingsChange(){
    update();
}

void GLRenderer::keyPressEvent(QKeyEvent *event){
    std::cout<<"Key Press"<<std::endl;
    update();
}

void GLRenderer::mouseMoveEvent(QMouseEvent *event){
    int posX = event->position().x();
    int posY = event->position().y();
}
