#include "myglwidget.h"

#include <GL/GLU.h>
#include <QSurfaceFormat>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <iostream>
#include<windows.h>

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    isMouseDragged = false;
    isDoubleClickMouseDragged = false;
    zoom = 1;
    anglex = 0;
    angley = 0;
}

void MyGLWidget::incrementCurrentfile()
{
    this->currentFile += 1;
    this->currentFile = this->currentFile % sphr.getN();
    emit this->updateCurrentfile(this->currentFile);
    std::cout << "Emit Current file : " << this->currentFile << std::endl;
    this->update();
}

void MyGLWidget::decrementCurrentfile()
{
    this->currentFile -= 1;
    if(this->currentFile < 0)
        this->currentFile = sphr.getN()-1;
    emit this->updateCurrentfile(this->currentFile);
    std::cout << "Emit Current file : " << this->currentFile << std::endl;
    this->update();
}

void MyGLWidget::play()
{
    this->animation = true;
    this->update();
}

void MyGLWidget::stop()
{
    this->animation = false;
    this->update();
}

void MyGLWidget::reset()
{
    this->currentFile = 0;
    this->update();
    emit this->updateCurrentfile(this->currentFile);
    std::cout << "Emit Current file : " << this->currentFile << std::endl;
}

void MyGLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();
}

void MyGLWidget::drawCylinder(double r)
{
    int lats = 100;
    int longs = 100;
    double h = 3;
    int i, j;
    double x1, y1, z1, x2, y2, z2;
    for(i = 0; i <= lats ; i++) {
        x1 = r*cos(2 * M_PI / lats * i);
        y1 = r*sin(2 * M_PI / lats * i);
        x2 = r*cos(2 * M_PI / lats * (i+1));
        y2 = r*sin(2 * M_PI / lats * (i+1));
        // z
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_STRIP);
        for(j = 0 ; j < longs ; j++) {
            z1 = -h/2 + h/longs * j;
            z2 = -h/2 + h/longs * (j+1);
            glNormal3f(x1, y1, 0);
            glVertex3f(x1, y1, z1);
            glNormal3f(x2, y2, 0);
            glVertex3f(x2, y2, z1);
            glNormal3f(x2, y2, 0);
            glVertex3f(x2, y2, z2);
            glNormal3f(x1, y1, 0);
            glVertex3f(x1, y1, z2);
            glNormal3f(x1, y1, 0);
            glVertex3f(x1, y1, z1);
        }
        glEnd();

        // x
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLE_STRIP);
        for(j = 0 ; j < longs ; j++) {
            z1 = -h/2 + h/longs * j;
            z2 = -h/2 + h/longs * (j+1);
            glNormal3f(0, x1, y1);
            glVertex3f(z1, x1, y1);
            glNormal3f(0, x2, y2);
            glVertex3f(z1, x2, y2);
            glNormal3f(0, x2, y2);
            glVertex3f(z2, x2, y2);
            glNormal3f(0, x1, y1);
            glVertex3f(z2, x1, y1);
            glNormal3f(0, x1, y1);
            glVertex3f(z1, x1, y1);
        }
        glEnd();

        // y
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLE_STRIP);
        for(j = 0 ; j < longs ; j++) {
            z1 = -h/2 + h/longs * j;
            z2 = -h/2 + h/longs * (j+1);
            glNormal3f(y1, 0, x1);
            glVertex3f(y1, z1, x1);
            glNormal3f(y2, 0, x2);
            glVertex3f(y2, z1, x2);
            glNormal3f(y2, 0, x2);
            glVertex3f(y2, z2, x2);
            glNormal3f(y1, 0, x1);
            glVertex3f(y1, z2, x1);
            glNormal3f(y1, 0, x1);
            glVertex3f(y1, z1, x1);
        }
        glEnd();
    }
}


void MyGLWidget::paintGL()
{

    prepare();
    std::cout << "zoom : " << zoom << std::endl;
    std::cout << "anglex : " << anglex << std::endl;
    std::cout << "angley : " << angley << std::endl;
    std::cout << "posX : " << posX << std::endl;
    std::cout << "posY : " << posY << std::endl;
    GLfloat lightPosition[4] = {3 , 10 , 3, 1};
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glTranslated(posX, -posY, 0);

    glRotatef(-angley,1.0,0.0,0.0);
    glRotatef(-anglex,0.0,1.0,0.0);

    glScalef(zoom,zoom,zoom);


    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    sphr.Draw(currentFile);
    if(drawContainer) {
        plr.Draw(currentFile, isLineContainer);
    }
    if(animation)
       run();
    glFinish();
}


void MyGLWidget::run() {
    this->currentFile += 1;
    this->currentFile = this->currentFile % sphr.getN();
    std::cout << "Current file : " << this->currentFile << std::endl;
    this->update();
    emit this->updateCurrentfile(this->currentFile);
}

void MyGLWidget::loadDirectory(std::string directory)
{
    sphr.load(directory);
    plr.load(directory);
    this->reset();
}

void MyGLWidget::on_new_container_visual_parameter(std::string directory)
{
    if(directory.compare("Full Container") == 0) {
        drawContainer = true;
        isLineContainer = false;
    } else if(directory.compare("Line Container") == 0) {
        drawContainer = true;
        isLineContainer = true;
    } else {
        drawContainer = false;
    }
    update();
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    mousePosX = event->pos().x();
    mousePosY = event->pos().y();
    if(event->button() == Qt::MouseButton::LeftButton)
        isMouseDragged = true;
    std::cout << "Down on ("<< event->pos().x() << ", "<< event->pos().y() << ")\n" << std::endl;
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        isMouseDragged = false;
        isDoubleClickMouseDragged = false;
    }
    if(event->button() == Qt::MouseButton::MiddleButton) {
        zoom = 1;
        anglex = 0;
        angley = 0;
        posX = 0;
        posY = 0;
        update();
    }
    std::cout << "Release on ("<< event->pos().x() << ", "<< event->pos().y() << ")\n" << std::endl;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDragged) {
        double dx =  (double)(event->pos().x()-mousePosX)/5;
        double dy =  (double)(event->pos().y()-mousePosY)/5;

        std::cout << "Move displacement ("<< event->pos().x()-mousePosX << ", "<< event->pos().y()-mousePosY << ")\n" << std::endl;
        mousePosX = event->pos().x();
        mousePosY = event->pos().y();

        anglex += dx;
        if(anglex > 360)
            anglex -= 360;
        if(anglex < 0)
            anglex += 360;
        angley += dy;
        update();
    } else if (isDoubleClickMouseDragged) {
        double dx =  (double)(event->pos().x()-mousePosX)/100;
        double dy =  (double)(event->pos().y()-mousePosY)/100;
        std::cout << "d ("<< dx << ", "<< dy << ")\n" << std::endl;
        mousePosX = event->pos().x();
        mousePosY = event->pos().y();
        posX += dx;
        posY += dy;
        update();
    }
}

void MyGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    std::cout << "Double click" << std::endl;
    isDoubleClickMouseDragged = true;
    mousePosX = event->pos().x();
    mousePosY = event->pos().y();
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    std::cout << "Wheel rolls " << event->angleDelta().ry() << std::endl;
    double D = event->angleDelta().ry();
    if(D > 0){
        zoom = zoom*(fabs(D/600)+1);
    }
    if(D < 0){
        zoom = zoom/(fabs(D/600)+1);
    }
    update();
}

void MyGLWidget::prepare()
{
    GLfloat mat_specular[4] = { (GLfloat)(0.5), (GLfloat)(0.5), (GLfloat)(0.5), (GLfloat)(1.0) };
    GLfloat light_ambient[4] = {(GLfloat)(0.3), (GLfloat)(0.3),(GLfloat)(0.3), (GLfloat)(1.0) };

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_specular);

    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    glMaterialf(GL_FRONT, GL_SHININESS, 100);


    glHint(GL_FOG_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,.0f);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,.0f);
}

