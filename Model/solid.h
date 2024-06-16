#ifndef SOLID_H
#define SOLID_H

#include<cstdio>
#include<vector>
#include "velocity.h"

class Solid
{
public:
    Solid();
    void ReadFromFile(FILE *ft);
protected:
    std::vector<double> x{},y{},z{};
    std::vector<double> nx{},ny{},nz{};
    std::vector<double> tx{},ty{},tz{};
    std::vector<double> sx{},sy{},sz{};
    std::vector<Velocity> vx{},vy{},vz{};
    std::vector<Velocity> wx{},wy{},wz{};
    double beta;
    double orx,ory,orz;
    double Vx,Vy,Vz,Wx,Wy,Wz;
    int Ngb;
    int N,Nold;
};

#endif // SOLID_H
