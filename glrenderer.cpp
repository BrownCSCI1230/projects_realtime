#include "glrenderer.h"

#include <QCoreApplication>
#include "sphere.h"
#include "cube.h"
#include "settings.h"

GLRenderer::GLRenderer(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_prev_mouse_pos = glm::vec2(size().width()/2, size().height()/2);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void GLRenderer::finish()
{
    m_shader.finish();
    m_fboShader.finish();
    m_fbo.finish();
    m_fullscreen_vbo->finish();
    m_fullscreen_vao->finish();
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
    m_fboShader.initialize(ShaderLoader::createShaderProgram("Resources/Shaders/postprocessing.vert", "Resources/Shaders/postprocessing.frag"));
    m_fbo.initialize(size().width(), size().height());
    std::cout<<"FBO initialized"<<std::endl;

    std::vector<GLfloat> fullscreen_quad =
    {   //    POSITIONS    //    UVs    //
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f
    };

    m_fullscreen_vbo = std::make_shared<vbo>(fullscreen_quad);
    std::cout<<"VBO initialized"<<std::endl;
    m_fullscreen_vao = std::make_shared<vao>(m_fullscreen_vbo, VAOType::POS_UV);
    std::cout<<"VAO initialized"<<std::endl;

    //Load Scene Data
    CS123::CS123SceneLoader::load("Resources/SceneFiles/Parsing_Lab_City.xml", m_metaData);

    //Add shapes to vao array
    Sphere sph = Sphere(20, 20);
    std::shared_ptr<vbo> sphere_vbo = std::make_shared<vbo>(sph.generateShape());
    std::shared_ptr<vao> sphere_vao = std::make_shared<vao>(sphere_vbo, VAOType::POS_NORM);
    m_vaos.push_back(sphere_vao);

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

    //Set Camera Uniforms
    m_shader.setUniformMat4("view", m_cam.getView());
    m_shader.setUniformMat4("projection", m_cam.getProjection());

    //Set Global Uniforms
    m_shader.setUniform1f("ambientIntensity", m_metaData.globalData.ka);
    m_shader.setUniform1f("diffuseIntensity", m_metaData.globalData.kd);
    m_shader.setUniform1f("specularIntensity", m_metaData.globalData.ks);

    //Set Lights
    m_shader.setUniform1i("numLights", m_metaData.lights.size());
    for(int j = 0; j<m_metaData.lights.size(); j++){
        if(m_metaData.lights[j].type == LightType::LIGHT_DIRECTIONAL){
            m_shader.setUniform1i("lightType["+std::to_string(j)+"]", 0);
            m_shader.setUniformVec4("worldSpace_lightDir["+std::to_string(j)+"]", m_metaData.lights[j].dir);
            m_shader.setUniformVec3("lightIntensity["+std::to_string(j)+"]", m_metaData.lights[j].function);
        }
        if(m_metaData.lights[j].type == LightType::LIGHT_POINT){
            m_shader.setUniform1i("lightType["+std::to_string(j)+"]", 1);
            m_shader.setUniformVec4("worldSpace_lightPos["+std::to_string(j)+"]", m_metaData.lights[j].pos);
            m_shader.setUniformVec3("lightIntensity["+std::to_string(j)+"]", m_metaData.lights[j].function);
        }
    }

    //Set Shapes
    for(int i = 0; i<m_metaData.shapes.size(); i++){
        m_shader.setUniformMat4("model", m_metaData.shapes[i].ctm);
        m_shader.setUniformVec4("objectColor", m_metaData.shapes[i].primitive.material.cDiffuse);
        if(m_metaData.shapes[i].primitive.type == PrimitiveType::PRIMITIVE_CUBE){
            m_vaos[0]->draw(m_shader);
        }
        else{
            m_vaos[0]->draw(m_shader);
        }
    }

//    m_fbo.unbindFBO();
//    glClear(GL_COLOR_BUFFER_BIT);
//    m_fboShader.bind();
//    m_fboShader.setUniform1i("invertColors", settings.invertColors);
//    m_fboShader.setUniform1i("sharpenImage", settings.sharpenImage);
//    m_fbo.bindTexture();
//    //glDisable(GL_DEPTH_TEST);
//    m_fullscreen_vao->draw(m_fboShader);
}

void GLRenderer::resizeGL(int w, int h)
{
    //m_cam->resize(w, h);
    update();
}

void GLRenderer::settingsChange(){
    update();
}

void GLRenderer::keyPressEvent(QKeyEvent *event){
    std::cout<<"Key Press"<<std::endl;
    m_cam.onKeyPressed(event);
    update();
}

void GLRenderer::mouseMoveEvent(QMouseEvent *event){
    int posX = event->position().x();
    int posY = event->position().y();
    int deltaX = posX - m_prev_mouse_pos.x;
    int deltaY = posY - m_prev_mouse_pos.y;

    m_prev_mouse_pos = glm::vec2(posX, posY);
    if(event->buttons() == Qt::RightButton){
        m_cam.mouseMoved(deltaX, deltaY);
        update();
    }
}
