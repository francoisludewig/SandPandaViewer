#ifndef DISK_H
#define DISK_H

#include "solid.h"
#include<cstdio>

class Disk : Solid
{
public:
    Disk();
    void ReadFromFile(FILE *ft);
    void Draw(int n, bool isLineContainer);

private:
    double r;
    int periodic;
};

#endif // DISK_H
