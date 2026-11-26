#ifndef SPHEREREPOSITORY_H
#define SPHEREREPOSITORY_H

#include "Model/sphere.h"
#include <vector>
#include <string>
#include <functional>

class SphereRepository
{
public:
    SphereRepository();
    void Draw(int n);
    int getN() const {return N;}

    // Nombre de fichiers grain_0.txt, grain_1.txt, ... consécutifs dans <directory>/Out
    static int countFiles(const std::string& directory);

    // Charge les fichiers grain_<n>.txt pour chaque n de fileNumbers.
    // Le premier fichier est lu seul, les suivants en parallèle sur plusieurs threads.
    // onFileLoaded(k) est appelé (un appel à la fois, dans un ordre quelconque) après
    // la lecture du fichier d'indice k dans fileNumbers ; s'il renvoie false, le
    // chargement est interrompu et load renvoie false.
    // Si un fichier est illisible, seuls les fichiers qui le précèdent sont conservés.
    bool load(const std::string& directory,
              const std::vector<int>& fileNumbers,
              const std::function<bool(int)>& onFileLoaded = {});
private:
    std::vector<Sphere> spheres {};
    int N = 0;
};

#endif // SPHEREREPOSITORY_H
