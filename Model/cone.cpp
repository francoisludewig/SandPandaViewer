#include "cone.h"

#include <QOpenGLFunctions>
#include <GL/glu.h>
#include <limits>
#include <iostream>

Cone::Cone():Solid() {
    r0 = 0.25;
    r1 = 0.5;
    h = 0.75;
    dr = 0.25;
}

void Cone::ReadFromFile(FILE *ft)
{
    int b;
    Solid::ReadFromFile(ft);
    fscanf(ft,"%lf\t%lf\t%lf\t%lf",&h,&r0,&r1,&dr);
    fscanf(ft,"%d\t%d\t%d",&inte,&b,&Ngb);
}

bool Cone::readFromStream(std::istream& in) {
    int b = 0;

    // Lire la partie commune depuis la classe de base
    if (!Solid::readFromStream(in)) {
        std::cerr << "Erreur lors de la lecture de Solid depuis Cone\n";
        return false;
    }

    // Lecture des données spécifiques à Cone
    if (!(in >> h >> r0 >> r1 >> dr)) {
        std::cerr << "Erreur lecture des dimensions Cone\n";
        return false;
    }

    if (!(in >> inte >> b >> Ngb)) {
        std::cerr << "Erreur lecture des entiers Cone\n";
        return false;
    }

    // Consommer le retour à la ligne restant (si présent)
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return true;
}

void Cone::Display() {
    Solid::Display();

    printf("(h, r0, r1, dr) = (%e\t%e\t%e\t%e)\n\n",h,r0,r1,dr);
}

void Cone::Draw(int n, bool isLineContainer)
{
    double PI = 3.1415926;
    double nlx,nly,nlz;
    nlx = ny[n];
    nly = nz[n];
    nlz = -nx[n];
    double theta;
    double cost = nlz;
    double sint = sqrt(nlx*nlx+nly*nly);
    if(sint >= 0)
        theta = acos(cost);
    else
        theta = 2*PI-acos(cost);
    //NSLog(@"theta = %e",theta);
    glPushMatrix();
    glTranslatef(y[n]-h/2*nlx,z[n]-h/2*nly,-x[n]-h/2*nlz);
    glPushMatrix();
    glRotatef(-theta/2/PI*360,nly,-nlx,0);
    GLUquadric* quad = gluNewQuadric();
    if(inte == 0){
        glColor3f(0.5, 0.5, 1);

        if(isLineContainer)
            gluQuadricDrawStyle(quad, GLU_LINE);

        gluQuadricOrientation(quad, GLU_INSIDE);
        gluCylinder(quad, r0, r1, h, 50, 5);
    }
    else{
        glColor3f(0, 0, 1);

        if(isLineContainer)
            gluQuadricDrawStyle(quad, GLU_LINE);

        gluQuadricOrientation(quad, GLU_OUTSIDE);
        gluCylinder(quad, r0*0.999, r1-(r0*0.001), h, 50, 5);
    }
    if(inte == 0){
        glColor3f(0, 0, 1);

        if(isLineContainer)
            gluQuadricDrawStyle(quad, GLU_LINE);

        gluQuadricOrientation(quad, GLU_OUTSIDE);
        gluCylinder(quad, r0*1.001, r1+(r0*0.001), h, 50, 5);
    }
    else{
        glColor3f(0.5, 0.5, 1);

        if(isLineContainer)
            gluQuadricDrawStyle(quad, GLU_LINE);

        gluQuadricOrientation(quad, GLU_INSIDE);
        gluCylinder(quad, r0, r1, h, 50, 5);
    }
    glPopMatrix();
    glPopMatrix();
}
