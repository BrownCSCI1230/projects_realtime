#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCheckBox>
#include "glrenderer.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void initialize();
    void finish();

private:
    void connectInvertColors();
    void connectSharpenImage();
    GLRenderer *glRenderer;
    QCheckBox *invertColors;
    QCheckBox *sharpenImage;

private slots:
    void onInvertColors();
    void onSharpenImage();
};
#endif // MAINWINDOW_H
