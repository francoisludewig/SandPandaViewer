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
    int getN() {return N;}

    // Nombre de fichiers grain_0.txt, grain_1.txt, ... consécutifs dans <directory>/Out
    static int countFiles(const std::string& directory);

    // Charge les fichiers grain_<n>.txt pour chaque n de fileNumbers (dans l'ordre).
    // onFileLoaded(k) est appelé après chaque fichier (k = nombre de fichiers lus) ;
    // s'il renvoie false, le chargement est interrompu et load renvoie false.
    bool load(const std::string& directory,
              const std::vector<int>& fileNumbers,
              const std::function<bool(int)>& onFileLoaded = {});
private:
    std::vector<Sphere> spheres {};
    int N = 0;
};

#endif // SPHEREREPOSITORY_H
