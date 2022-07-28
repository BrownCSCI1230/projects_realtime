#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings {
    int shapeParameter1 = 1;
    int shapeParameter2 = 1;
    bool invertColors = false;
    bool sharpenImage = false;
};


// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
