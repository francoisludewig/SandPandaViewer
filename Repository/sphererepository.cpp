#include "Repository/sphererepository.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdio>
#include <limits>

SphereRepository::SphereRepository() {

}

int SphereRepository::countFiles(const std::string& directory)
{
    namespace fs = std::filesystem;

    int n = 0;
    std::error_code ec;
    while (fs::exists(fs::path(directory) / "Out" / ("grain_" + std::to_string(n) + ".txt"), ec))
        ++n;
    return n;
}

bool SphereRepository::load(const std::string& directory,
                            const std::vector<int>& fileNumbers,
                            const std::function<bool(int)>& onFileLoaded)
{
    namespace fs = std::filesystem;

    spheres.clear();
    N = 0;
    const int M = static_cast<int>(fileNumbers.size());

    for (int k = 0; k < M; ++k) {
        fs::path filePath = fs::path(directory) / "Out" / ("grain_" + std::to_string(fileNumbers[k]) + ".txt");

        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Cannot open " << filePath << "\n";
            break;
        }

        int count = 0;
        file >> count;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip to next line

        if (k == 0) {
            spheres.reserve(count);
            for (int i = 0; i < count; ++i) {
                Sphere sph{M};
                if (!sph.readFromStream(file, k)) {
                    std::cerr << "Error reading sphere " << i << " in file " << filePath << "\n";
                }
                spheres.push_back(std::move(sph));
            }
        } else {
            for (int i = 0; i < count && i < static_cast<int>(spheres.size()); ++i) {
                if (!spheres[i].readFromStream(file, k)) {
                    std::cerr << "Error updating sphere " << i << " in file " << filePath << "\n";
                }
            }
        }

        ++N;
        if (onFileLoaded && !onFileLoaded(N))
            return false;
    }

    std::cout << N << " grain files loaded.\n";
    return true;
}


void SphereRepository::Draw(int n) {
    if(n >=0 && n < N) {
        for(auto sph : spheres)
            sph.Draw(n);
    }
}


