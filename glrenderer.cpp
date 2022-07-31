#include "glrenderer.h"

#include <QCoreApplication>
#include <set>
#include "sphere.h"
#include "cylinder.h"
#include "cone.h"
#include "cube.h"
#include "mesh.h"
#include "settings.h"

const uint SPHERE_VAO_INDEX = 0;
const uint CUBE_VAO_INDEX = 1;
const uint CONE_VAO_INDEX = 2;
const uint CYLINDER_VAO_INDEX = 3;

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
    CS123::CS123SceneLoader::load("Resources/SceneFiles/all_objects.xml", m_metaData);

    //Initialize Primitive VAOs

    //Add Sphere data to vao vector
    Sphere sph = Sphere(30, 30);
    std::shared_ptr<vbo> sphere_vbo = std::make_shared<vbo>(sph.generateShape());
    std::shared_ptr<vao> sphere_vao = std::make_shared<vao>(sphere_vbo, VAOType::POS_NORM_UV);
    m_vaos.push_back(sphere_vao);

    //Add Cube data to vao vector
    Cube cube = Cube(2);
    std::shared_ptr<vbo> cube_vbo = std::make_shared<vbo>(cube.generateShape());
    std::shared_ptr<vao> cube_vao = std::make_shared<vao>(cube_vbo, VAOType::POS_NORM);
    m_vaos.push_back(cube_vao);

    Cone cone = Cone(20, 5);
    std::shared_ptr<vbo> cone_vbo = std::make_shared<vbo>(cone.generateShape());
    std::shared_ptr<vao> cone_vao = std::make_shared<vao>(cone_vbo, VAOType::POS_NORM);
    m_vaos.push_back(cone_vao);

    Cylinder cyl = Cylinder(20, 1);
    std::shared_ptr<vbo> cylinder_vbo = std::make_shared<vbo>(cyl.generateShape());
    std::shared_ptr<vao> cylinder_vao = std::make_shared<vao>(cylinder_vbo, VAOType::POS_NORM);
    m_vaos.push_back(cylinder_vao);

    //Initialize Mesh VAOs
    int vaoIndex = 4;
    std::set<std::string> uniqueMeshFiles = {};
    // generate list of unique meshfiles used within scene
    for (auto& shape : m_metaData.shapes) {
        if (shape.primitive.type == PrimitiveType::PRIMITIVE_MESH) {
            uniqueMeshFiles.insert(shape.primitive.meshfile);
        }
    }
    // using unique list, load each mesh once into a vao
    for (auto& meshfile : uniqueMeshFiles) {
        Mesh mesh = Mesh(meshfile);
        std::shared_ptr<vbo> mesh_vbo = std::make_shared<vbo>(mesh.generateShape());
        std::shared_ptr<vao> mesh_vao = std::make_shared<vao>(mesh_vbo, VAOType::POS_NORM);
        m_vaos.push_back(mesh_vao);

        m_meshLookup[meshfile] = vaoIndex;
        vaoIndex++;
    }

    //Initialize Texture Data
    std::set<std::string> uniqueTextureFiles = {};
    // get unique texture files
    for (auto& shape : m_metaData.shapes) {
        if (shape.primitive.material.textureMap.isUsed) {
            uniqueTextureFiles.insert(shape.primitive.material.textureMap.filename);
        }
    }
    // using unique list, load each texture once
    for (auto& textureFile : uniqueTextureFiles) {
        m_textures[textureFile] = std::make_unique<Texture>();
        // default for now is always 0, theoretically should be set per shape
        m_textures[textureFile]->initialize(textureFile, 0);
    }

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
    m_shader.setUniformMat4("view", m_cam.getView());
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
        if(m_metaData.lights[j].type == LightType::LIGHT_POINT){
            m_shader.setUniform1i("lightType["+std::to_string(j)+"]", 0);
            m_shader.setUniformVec4("camSpace_lightPos["+std::to_string(j)+"]", view * m_metaData.lights[j].pos);
            m_shader.setUniformVec4("lightColor["+std::to_string(j)+"]", m_metaData.lights[j].color);
            m_shader.setUniformVec3("lightFunction["+std::to_string(j)+"]", m_metaData.lights[j].function);
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


        m_shader.setUniform1i("texUsed",shape.primitive.material.textureMap.isUsed);

        if (shape.primitive.material.textureMap.isUsed) {
            Texture* tex = m_textures[shape.primitive.material.textureMap.filename].get();
            tex->bind();
            m_shader.setUniform1i("objTexture",tex->getTextureUnit());
        }

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
            case PrimitiveType::PRIMITIVE_MESH: {
                int vaoIndex = m_meshLookup[shape.primitive.meshfile];
                m_vaos[vaoIndex]->draw(m_shader);
                break;
            }
            default:
                break;
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
    m_cam.resize(w, h);
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
    if(event->buttons() == Qt::LeftButton){
        m_cam.mouseMoved(deltaX, deltaY);
        update();
    }
}
