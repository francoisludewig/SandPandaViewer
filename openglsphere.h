#ifndef OPENGLSPHERE_H
#define OPENGLSPHERE_H

#include<vector>
#include <QOpenGLFunctions>

class OpenGLSphere
{
public:
    OpenGLSphere();
    void draw(GLfloat x, GLfloat y, GLfloat z, double radius);

private:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;
};

#endif // OPENGLSPHERE_H
