#include "Repository/sphererepository.h"

#include <cstdio>

SphereRepository::SphereRepository() {

}

void SphereRepository::load(std::string directory)
{
    int count = 0;
    N = 0;
    std::FILE *ft = nullptr;
    char filename[2048];

    spheres.clear();

    do {
        //sprintf(filename, "C:\\Users\\utilisateur\\Documents\\TestBox\\Out\\grain_%d.txt", N);
        //sprintf(filename, "C:\\Users\\utilisateur\\OneDrive - student.helmo.be\\Bureau\\Dev\\Archives\\LuGaProjects\\Test\\Sphere\\Elbow\\New\\Out\\grain_%d.txt", N);
        //sprintf(filename, "C:\\Users\\utilisateur\\OneDrive - student.helmo.be\\Bureau\\Dev\\Archives\\LuGaProjects\\Test\\Sphere\\Cone\\New\\Out\\grain_%d.txt", N);
        sprintf(filename, "%s\\Out\\grain_%d.txt", directory.data(), N);

        printf("filename : %s\n", filename);
        ft = std::fopen(filename,"r");
        if(ft) {
            printf("Load file %s\n", filename);
            fscanf_s(ft,"%d",&count);
            if(N == 0) {
                for(int i = 0 ; i < count ; ++i) {
                    Sphere sph{};
                    sph.ReadFromFile(ft);
                    spheres.push_back(sph);
                }
            } else {
                for(int i = 0 ; i < count ; ++i) {
                    spheres[i].ReadFromFile(ft);
                }
            }
            printf("Sphere count : %d\n", count);
            fclose(ft);
            N++;
        }
    } while(ft);

    printf("%d files found and loaded.\n", N);
}


void SphereRepository::Draw(int n) {
    if(n >=0 && n < N) {
        for(auto sph : spheres)
            sph.Draw(n);
    }
}


