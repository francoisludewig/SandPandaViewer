#include "plan.h"

#include <QOpenGLFunctions>
#include </usr/include/GL/glu.h>
#include <limits>
#include <iostream>

Plan::Plan() : Solid() {
    dt = 0.5;
    ds = 0.5;
    ped = -9;
    inAndOut = 0;
}

void Plan::ReadFromFile(FILE *ft)
{
    double a;
    Solid::ReadFromFile(ft);
    fscanf(ft,"%lf\t%lf\t%lf\n%d\t%d\t%d\n",&a,&dt,&ds,&ped,&Ngb,&inAndOut);
}

bool Plan::readFromStream(std::istream& in) {
    double a = 0;

    // Lecture de la partie commune (héritée de Solid)
    if (!Solid::readFromStream(in)) {
        std::cerr << "Plan : erreur lors de la lecture de la partie Solid\n";
        return false;
    }

    // Lecture de la ligne contenant a, dt, ds
    if (!(in >> a >> dt >> ds)) {
        std::cerr << "Plan : erreur de lecture (a, dt, ds)\n";
        return false;
    }

    // Consomme le retour à la ligne
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Lecture de la ligne contenant ped, Ngb, inAndOut
    if (!(in >> ped >> Ngb >> inAndOut)) {
        std::cerr << "Plan : erreur de lecture (ped, Ngb, inAndOut)\n";
        return false;
    }

    // Consomme le retour à la ligne
    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return true;
}


void Plan::Draw(int n, bool isLineContainer)
{
    printf("Plan::Draw n = %d (%f, %f, %f) - n (%f, %f, %f)\n", n, x[n], y[n], z[n], nx[n], ny[n], nz[n]);
    double nl;
    nl = sqrt(dt*dt+ds*ds)/1000.;

    if(inAndOut == 0){
        glColor3f(0, 0, 1);
    }
    else{
        glColor3f(0.5, 0.5, 1);
    }
    glPushMatrix();
    glTranslated(y[n], z[n], -x[n]);
    if(isLineContainer)
        glBegin(GL_LINE_LOOP);
    else
        glBegin(GL_POLYGON);
    glNormal3f(-ny[n], -nz[n],nx[n]);
    glVertex3f(-ny[n]*nl-dt/2*ty[n]-ds/2*sy[n], -nz[n]*nl-dt/2*tz[n]-ds/2*sz[n],-(-nx[n]*nl-dt/2*tx[n]-ds/2*sx[n]));
    glVertex3f(-ny[n]*nl-dt/2*ty[n]+ds/2*sy[n], -nz[n]*nl-dt/2*tz[n]+ds/2*sz[n],-(-nx[n]*nl-dt/2*tx[n]+ds/2*sx[n]));
    glVertex3f(-ny[n]*nl+dt/2*ty[n]+ds/2*sy[n], -nz[n]*nl+dt/2*tz[n]+ds/2*sz[n],-(-nx[n]*nl+dt/2*tx[n]+ds/2*sx[n]));
    glVertex3f(-ny[n]*nl+dt/2*ty[n]-ds/2*sy[n], -nz[n]*nl+dt/2*tz[n]-ds/2*sz[n],-(-nx[n]*nl+dt/2*tx[n]-ds/2*sx[n]));
    glEnd();
    glPopMatrix();

    glColor3f(0.5, 0.5, 1);

    glPushMatrix();
    glTranslated(y[n], z[n], -x[n]);
    if(isLineContainer)
        glBegin(GL_LINE_LOOP);
    else
        glBegin(GL_POLYGON);
    glNormal3f(-ny[n], -nz[n],nx[n]);
    glVertex3f(-dt/2*ty[n]-ds/2*sy[n], -dt/2*tz[n]-ds/2*sz[n],-(-dt/2*tx[n]-ds/2*sx[n]));
    glVertex3f(-dt/2*ty[n]+ds/2*sy[n], -dt/2*tz[n]+ds/2*sz[n],-(-dt/2*tx[n]+ds/2*sx[n]));
    glVertex3f(+dt/2*ty[n]+ds/2*sy[n], +dt/2*tz[n]+ds/2*sz[n],-(+dt/2*tx[n]+ds/2*sx[n]));
    glVertex3f(+dt/2*ty[n]-ds/2*sy[n], +dt/2*tz[n]-ds/2*sz[n],-(+dt/2*tx[n]-ds/2*sx[n]));
    glEnd();
    glPopMatrix();
}
