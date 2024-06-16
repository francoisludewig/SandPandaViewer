#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QPushButton>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <string>
#include "Repository/sphererepository.h"
#include "Repository/containerrepository.h"

class MyGLWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
    MyGLWidget(QWidget *parent);

    void incrementCurrentfile();
    void decrementCurrentfile();
    void play();
    void stop();
    void reset();

protected:
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    int mousePosX, mousePosY;
    bool isMouseDragged;
    bool isDoubleClickMouseDragged;
    SphereRepository sphr {};
    ContainerRepository plr {};
    int currentFile = 0;
    bool animation = false;
    bool drawContainer = true;
    bool isLineContainer = false;

    void prepare();
    double posX = 0, posY = 0;
    double zoom;
    double anglex;
    double angley;
    void drawCylinder(double r);
    void run();

signals:
    void updateCurrentfile(int currentfile);

public slots:
    void loadDirectory(std::string directory);
    void on_new_container_visual_parameter(std::string directory);
};

#endif // MYGLWIDGET_H
