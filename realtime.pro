QT += widgets opengl openglwidgets gui xml

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

unix:!macx{
    LIBS += lGLU
}

win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += \
    Parsing/cs123sceneloader.cpp \
    Parsing/cs123xmlsceneparser.cpp \
    camera.cpp \
    glrenderer.cpp \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    glew-2.2.0/src/glew.c

HEADERS += \
    Parsing/cs123isceneparser.h \
    Parsing/cs123scenedata.h \
    Parsing/cs123sceneloader.h \
    Parsing/cs123xmlsceneparser.h \
    ShaderLoading/shaderloader.h \
    camera.h \
    glrenderer.h \
    mainwindow.h \
    settings.h \
    glew-2.2.0/include/GL/glew.h


# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += glm glew-2.2.0/include
DEPENDPATH += glm glew-2.2.0/include

DISTFILES += \
    Resources/SceneFiles/Test.xml \
    Resources/Shaders/default.frag \
    Resources/Shaders/default.vert \
    Resources/SceneFiles/Parsing_Lab_City.xml \
    Resources/SceneFiles/Test.xml \
    Resources/Shaders/phong.frag \
    Resources/Shaders/phong.vert \
    Resources/Shaders/postprocessing.frag \
    Resources/Shaders/postprocessing.vert
