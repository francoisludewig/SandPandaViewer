#ifndef CONE_H
#define CONE_H

#include "solid.h"
#include<cstdio>

class Cone : Solid
{
public:
    Cone();
    void ReadFromFile(FILE *ft);
    void Draw(int n, bool isLineContainer);
private:
    double r0,r1,h,dr;
    int inte;
};

#endif // CONE_H
