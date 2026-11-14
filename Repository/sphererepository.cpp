#include "Repository/sphererepository.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdio>

SphereRepository::SphereRepository() {

}

void SphereRepository::load(std::string directory)
{
    namespace fs = std::filesystem;

    spheres.clear();
    N = 0;
    int count = 0;

    while (true) {
        fs::path filePath = fs::path(directory) / "Out" / ("grain_" + std::to_string(N) + ".txt");

        std::ifstream file(filePath);
        if (!file.is_open())
            break;

        ++N;
    }
    std::cout << N << " files found.\n";

    for(int n = 0 ; n <= N ; ++n) {
        fs::path filePath = fs::path(directory) / "Out" / ("grain_" + std::to_string(n) + ".txt");

        std::ifstream file(filePath);
        if (!file.is_open())
            break;

        //std::cout << "Loading file: " << filePath << "\n";
        file >> count;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Skip to next line

        if (n == 0) {
            for (int i = 0; i < count; ++i) {
                Sphere sph{N};
                if (!sph.readFromStream(file, n)) {
                    std::cerr << "Error reading sphere " << i << " in file " << filePath << "\n";
                }
                spheres.push_back(sph);
            }
        } else {
            for (int i = 0; i < count; ++i) {
                if (i < spheres.size()) {
                    if (!spheres[i].readFromStream(file, n)) {
                        std::cerr << "Error updating sphere " << i << " in file " << filePath << "\n";
                    }
                }
            }
        }
    }

    std::cout << N << " files loaded.\n";
}


void SphereRepository::Draw(int n) {
    if(n >=0 && n < N) {
        for(auto sph : spheres)
            sph.Draw(n);
    }
}


