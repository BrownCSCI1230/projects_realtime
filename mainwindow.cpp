#include "mainwindow.h"
#include "settings.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow()
{
    glRenderer = new GLRenderer;

    QHBoxLayout *hLayout = new QHBoxLayout; // horizontal alignment
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(glRenderer, 1);
    this->setLayout(hLayout);

    // Create toggle for showing wireframe / normals
    invertColors = new QCheckBox();
    invertColors->setText(QStringLiteral("Invert Colors"));
    invertColors->setChecked(false);

    // Create toggle for showing wireframe / normals
    sharpenImage = new QCheckBox();
    sharpenImage->setText(QStringLiteral("Sharpen Image"));
    sharpenImage->setChecked(false);

    vLayout->addWidget(invertColors);
    vLayout->addWidget(sharpenImage);

    connectInvertColors();
    connectSharpenImage();
}

void MainWindow::initialize()
{
    glRenderer = new GLRenderer;

    QHBoxLayout *hLayout = new QHBoxLayout; // horizontal alignment
    QVBoxLayout *vLayout = new QVBoxLayout(); // vertical alignment
    vLayout->setAlignment(Qt::AlignTop);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(glRenderer, 1);
    this->setLayout(hLayout);

    // Create toggle for showing wireframe / normals
    invertColors = new QCheckBox();
    invertColors->setText(QStringLiteral("Invert Colors"));
    invertColors->setChecked(false);

    // Create toggle for showing wireframe / normals
    sharpenImage = new QCheckBox();
    sharpenImage->setText(QStringLiteral("Sharpen Image"));
    sharpenImage->setChecked(false);

    vLayout->addWidget(invertColors);
    vLayout->addWidget(sharpenImage);

    connectInvertColors();
    connectSharpenImage();
}

MainWindow::~MainWindow()
{
}

void MainWindow::finish()
{
    glRenderer->finish();
}

void MainWindow::connectInvertColors()
{
    connect(invertColors, &QCheckBox::clicked, this, &MainWindow::onInvertColors);
}

void MainWindow::onInvertColors()
{
    settings.invertColors = !settings.invertColors;
    glRenderer->settingsChange();
}

void MainWindow::connectSharpenImage()
{
    connect(sharpenImage, &QCheckBox::clicked, this, &MainWindow::onSharpenImage);
}

void MainWindow::onSharpenImage()
{
    settings.sharpenImage = !settings.sharpenImage;
    glRenderer->settingsChange();
}

