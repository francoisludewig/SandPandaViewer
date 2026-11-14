#include "Repository/sphererepository.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <cstdio>
#include <limits>
#include <algorithm>
#include <atomic>
#include <charconv>
#include <cstring>
#include <mutex>
#include <thread>

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

namespace {

// Lit tout le fichier en mémoire d'un seul coup (beaucoup plus rapide que ligne par ligne).
bool readWholeFile(const std::filesystem::path& path, std::string& buffer)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
        return false;
    file.seekg(0, std::ios::end);
    const std::streamoff size = file.tellg();
    if (size < 0)
        return false;
    buffer.resize(static_cast<size_t>(size));
    file.seekg(0, std::ios::beg);
    file.read(buffer.data(), size);
    return static_cast<bool>(file) || file.gcount() == size;
}

// Lit le prochain nombre (entier ou réel) avec std::from_chars :
// pas de locale, pas d'allocation, nettement plus rapide que operator>>.
inline bool nextNumber(const char*& p, const char* end, double& value)
{
    while (p < end && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n'))
        ++p;
    if (p >= end)
        return false;
    if (*p == '+')   // from_chars n'accepte pas le '+' initial
        ++p;
    const auto result = std::from_chars(p, end, value);
    if (result.ec != std::errc())
        return false;
    p = result.ptr;
    return true;
}

inline void skipLine(const char*& p, const char* end)
{
    while (p < end && *p != '\n')
        ++p;
    if (p < end)
        ++p;
}

constexpr int VALUES_PER_LINE = 18;   // b sp r x y z q0 q1 q2 q3 vx vy vz wx wy wz b b1

} // namespace

bool SphereRepository::load(const std::string& directory,
                            const std::vector<int>& fileNumbers,
                            const std::function<bool(int)>& onFileLoaded)
{
    namespace fs = std::filesystem;

    spheres.clear();
    N = 0;
    const int M = static_cast<int>(fileNumbers.size());
    if (M == 0)
        return true;

    auto pathOf = [&](int k) {
        return fs::path(directory) / "Out" / ("grain_" + std::to_string(fileNumbers[k]) + ".txt");
    };

    // Lit le fichier k et range ses valeurs à l'indice k de chaque sphère.
    // Les fichiers écrivent à des indices différents : plusieurs threads peuvent
    // donc lire des fichiers différents en même temps, sans verrou.
    auto parseFile = [&](int k, bool createSpheres) -> bool {
        std::string buffer;
        if (!readWholeFile(pathOf(k), buffer)) {
            std::cerr << "Cannot open " << pathOf(k) << "\n";
            return false;
        }
        const char* p = buffer.data();
        const char* end = p + buffer.size();

        double countValue = 0;
        if (!nextNumber(p, end, countValue)) {
            std::cerr << "Cannot read sphere count in " << pathOf(k) << "\n";
            return false;
        }
        skipLine(p, end);
        const int count = static_cast<int>(countValue);

        if (createSpheres) {
            spheres.reserve(count);
            for (int i = 0; i < count; ++i)
                spheres.emplace_back(M);
        }

        const int n = std::min(count, static_cast<int>(spheres.size()));
        double v[VALUES_PER_LINE];
        for (int i = 0; i < n && p < end; ++i) {
            // Chaque ligne est analysée séparément : une ligne erronée n'affecte pas les suivantes
            const char* lineEnd = static_cast<const char*>(std::memchr(p, '\n', end - p));
            if (!lineEnd)
                lineEnd = end;
            int j = 0;
            while (j < VALUES_PER_LINE && nextNumber(p, lineEnd, v[j]))
                ++j;
            p = (lineEnd < end) ? lineEnd + 1 : end;
            if (j < VALUES_PER_LINE) {
                std::cerr << "Sphere: parsing error, sphere " << i << " in file " << pathOf(k) << "\n";
                continue;
            }
            spheres[i].setFrame(k, static_cast<int>(v[1]), v[2],
                                v[3], v[4], v[5],
                                v[6], v[7], v[8], v[9],
                                v[10], v[11], v[12],
                                v[13], v[14], v[15]);
        }
        return true;
    };

    // Le premier fichier fixe le nombre de sphères : il est lu seul.
    if (!parseFile(0, true))
        return true;   // rien de chargé (N = 0)
    if (onFileLoaded && !onFileLoaded(0))
        return false;

    // Les autres fichiers sont répartis entre plusieurs threads.
    std::atomic<int> nextIndex{1};
    std::atomic<int> firstFailure{M};   // premier fichier illisible (les suivants seront ignorés)
    std::atomic<bool> canceled{false};
    std::mutex callbackMutex;

    auto worker = [&]() {
        while (!canceled) {
            const int k = nextIndex.fetch_add(1);
            if (k >= M || k >= firstFailure)
                return;
            if (!parseFile(k, false)) {
                int current = firstFailure.load();
                while (k < current && !firstFailure.compare_exchange_weak(current, k)) {}
                continue;
            }
            if (onFileLoaded) {
                std::lock_guard<std::mutex> lock(callbackMutex);
                if (!onFileLoaded(k))
                    canceled = true;
            }
        }
    };

    const unsigned hw = std::max(1u, std::thread::hardware_concurrency());
    const int threadCount = static_cast<int>(std::min<unsigned>(hw, 8));
    std::vector<std::thread> threads;
    for (int t = 1; t < threadCount; ++t)
        threads.emplace_back(worker);
    worker();   // le thread courant travaille aussi
    for (auto& t : threads)
        t.join();

    if (canceled)
        return false;

    N = firstFailure;
    std::cout << N << " grain files loaded.\n";
    return true;
}


void SphereRepository::Draw(int n) {
    if(n >=0 && n < N) {
        for(auto& sph : spheres)
            sph.Draw(n);
    }
}


