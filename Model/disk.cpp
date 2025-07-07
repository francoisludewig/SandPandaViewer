#include "disk.h"

#include <QOpenGLFunctions>
#include <limits>
#include <iostream>

Disk::Disk() {
    r = 0.5;
}

void Disk::ReadFromFile(FILE *ft)
{
    double a;
    printf("Disk : File - %d\n", ft);
    Solid::ReadFromFile(ft);
    fscanf(ft,"%lf\t%lf",&a,&r);
    fscanf(ft,"%d\t%d",&periodic,&Ngb);
}

bool Disk::readFromStream(std::istream& in) {
    double a = 0;

    std::cout << "Disk : Stream - " << &in << std::endl;

    // Partie héritée de Solid
    if (!Solid::readFromStream(in)) {
        std::cerr << "Disk : erreur lors de la lecture de la partie Solid\n";
        return false;
    }

    // Lecture des données spécifiques à Disk
    if (!(in >> a >> r)) {
        std::cerr << "Disk : erreur de lecture (a, r)\n";
        return false;
    }

    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (!(in >> periodic >> Ngb)) {
        std::cerr << "Disk : erreur de lecture (periodic, Ngb)\n";
        return false;
    }

    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return true;
}


void Disk::Display() {
    Solid::Display();
    printf("(r) = (%e)\n\n", r);
}


void Disk::Draw(int n, bool isLineContainer)
{
    double dn = r/1000;
    double theta,dtheta = 6.28318531/50.;

    glColor3f(0.5, 0.5, 1);

    glPushMatrix();
    glTranslated(y[n], z[n], -x[n]);
    if(isLineContainer)
        glBegin(GL_LINE_LOOP);
    else
        glBegin(GL_POLYGON);
    glNormal3f(ny[n], nz[n], -nx[n]);
    for(int i = 0 ; i <= 50 ; i++){
        theta = i*dtheta;
        glVertex3f(r*cos(theta)*ty[n]+r*sin(theta)*sy[n],r*cos(theta)*tz[n]+r*sin(theta)*sz[n],-(r*cos(theta)*tx[n]+r*sin(theta)*sx[n]));
    }
    glEnd();
    glPopMatrix();

    glColor3f(0, 0, 1);

    glPushMatrix();
    glTranslated(y[n], z[n], -x[n]);
    if(isLineContainer)
        glBegin(GL_LINE_LOOP);
    else
        glBegin(GL_POLYGON);
    glNormal3f(ny[n], nz[n], -nx[n]);
    for(int i = 0 ; i <= 50 ; i++){
        theta = i*dtheta;
        glVertex3f(-ny[n]*dn+r*cos(theta)*ty[n]+r*sin(theta)*sy[n],-nz[n]*dn+r*cos(theta)*tz[n]+r*sin(theta)*sz[n],-(-nx[n]*dn+r*cos(theta)*tx[n]+r*sin(theta)*sx[n]));

    }
    glEnd();
    glPopMatrix();
}
