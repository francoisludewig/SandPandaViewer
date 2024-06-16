#include "containerrepository.h"

ContainerRepository::ContainerRepository() {}

void ContainerRepository::Draw(int n, bool isLineContainer)
{
    if(n >=0 && n < N) {
        for(auto plan : plans)
            plan.Draw(n, isLineContainer);
        for(auto disk : disks)
            disk.Draw(n, isLineContainer);
        for(auto cone : cones)
            cone.Draw(n, isLineContainer);
        for(auto elbow : elbows)
            elbow.Draw(n, isLineContainer);
    }
}

void ContainerRepository::load(std::string directory)
{
    int Nplan = 0;
    int Ndisk = 0;
    int Ncone = 0;
    int Nelbow = 0;
    std::FILE *ft = nullptr;
    char filename[2048];
    N = 0;
    plans.clear();
    disks.clear();
    cones.clear();
    elbows.clear();

    do {
        //sprintf(filename, "C:\\Users\\utilisateur\\Documents\\TestBox\\Out\\grain_%d.txt", N);
        //sprintf(filename, "C:\\Users\\utilisateur\\OneDrive - student.helmo.be\\Bureau\\Dev\\Archives\\LuGaProjects\\Test\\Sphere\\Elbow\\New\\Out\\grain_%d.txt", N);
        //sprintf(filename, "C:\\Users\\utilisateur\\OneDrive - student.helmo.be\\Bureau\\Dev\\Archives\\LuGaProjects\\Test\\Sphere\\Cone\\New\\Out\\grain_%d.txt", N);
        sprintf(filename, "%s\\Out\\container_%d.txt", directory.data(), N);

        printf("filename : %s\n", filename);
        ft = std::fopen(filename,"r");
        if(ft) {
            printf("Load file %s\n", filename);
            fscanf_s(ft,"%d",&Nplan);
            fscanf_s(ft,"%d",&Ndisk);
            fscanf_s(ft,"%d",&Ncone);
            fscanf_s(ft,"%d",&Nelbow);
            if(N == 0) {
                for(int i = 0 ; i < Nplan ; ++i) {
                    Plan plan{};
                    plan.ReadFromFile(ft);
                    plans.push_back(plan);
                }
                for(int i = 0 ; i < Ndisk ; ++i) {
                    Disk disk{};
                    disk.ReadFromFile(ft);
                    disks.push_back(disk);
                }
                for(int i = 0 ; i < Ncone ; ++i) {
                    Cone cone{};
                    cone.ReadFromFile(ft);
                    cones.push_back(cone);
                }
                for(int i = 0 ; i < Nelbow ; ++i) {
                    Elbow elbow{};
                    elbow.ReadFromFile(ft);
                    elbows.push_back(elbow);
                }
            } else {
                for(int i = 0 ; i < Nplan ; ++i) {
                    plans[i].ReadFromFile(ft);
                }
                for(int i = 0 ; i < Ndisk ; ++i) {
                    disks[i].ReadFromFile(ft);
                }
                for(int i = 0 ; i < Ncone ; ++i) {
                    cones[i].ReadFromFile(ft);
                }
                for(int i = 0 ; i < Nelbow ; ++i) {
                    elbows[i].ReadFromFile(ft);
                }
            }

            printf("Plan count : %d\n", Nplan);
            fclose(ft);
            N++;
        }
    } while(ft);

    printf("%d files found and loaded.\n", N);
}
