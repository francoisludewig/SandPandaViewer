#ifndef CONTAINERREPOSITORY_H
#define CONTAINERREPOSITORY_H

#include <string>
#include <vector>
#include <functional>
#include "Model/plan.h"
#include "Model/disk.h"
#include "Model/cone.h"
#include "Model/elbow.h"

class ContainerRepository
{
public:
    ContainerRepository();
    void Draw(int n, bool isLineContainer);
    int getN() {return N;}

    // Charge les fichiers container_<n>.txt pour chaque n de fileNumbers (dans l'ordre).
    // Même convention que SphereRepository::load pour onFileLoaded.
    bool load(const std::string& directory,
              const std::vector<int>& fileNumbers,
              const std::function<bool(int)>& onFileLoaded = {});
private:
    std::vector<Plan> plans {};
    std::vector<Disk> disks {};
    std::vector<Cone> cones {};
    std::vector<Elbow> elbows {};
    int N = 0;
};

#endif // CONTAINERREPOSITORY_H
