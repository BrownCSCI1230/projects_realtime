#ifndef SETTINGS_H
#define SETTINGS_H


// Enumeration values for the Shapes that the user can select in the GUI.
enum ShapeType {
    SHAPE_TRIANGLE,
    SHAPE_CUBE,
    SHAPE_CONE,
    SHAPE_SPHERE,
    SHAPE_CYLINDER
};

struct Settings {
    int shapeType;
    int shapeParameter1 = 1;
    int shapeParameter2 = 1;
    bool invertColors = false;
    bool sharpenImage = false;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
