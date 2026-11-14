#ifndef SPHERE_H
#define SPHERE_H

#include<vector>
#include<cstdio>
#include <istream>


class Sphere
{
public:
    Sphere();
    Sphere(int size);

    void ReadFromFile(FILE *ft);
    bool readFromStream(std::istream& in);
    bool readFromStream(std::istream& in, int index);

    // Remplit l'état de la sphère pour l'image <index> (tableaux déjà dimensionnés).
    // q0..q3 : quaternion d'orientation. Utilisé par le lecteur rapide de SphereRepository.
    void setFrame(int index, int species, double radius,
                  double px, double py, double pz,
                  double q0, double q1, double q2, double q3,
                  double pvx, double pvy, double pvz,
                  double pwx, double pwy, double pwz);

    void Draw(int n);

private:
    std::vector<double> x{},y{},z{};
    std::vector<double> nx{},ny{},nz{};
    std::vector<double> tx{},ty{},tz{};
    std::vector<double> sx{},sy{},sz{};
    std::vector<double> vx{},vy{},vz{};
    std::vector<double> wx{},wy{},wz{};
    std::vector<int> sp{};
    std::vector<double> r{};
    bool bd = false;
};

#endif // SPHERE_H
