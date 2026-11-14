#include "Repository/simulationloader.h"

#include <QObject>

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <limits>

namespace {

// Taille (en octets) de <directory>/Out/<prefix><n>.txt pour chaque n ; 0 si absent.
std::vector<std::uintmax_t> fileSizes(const std::string& directory,
                                      const std::string& prefix,
                                      const std::vector<int>& fileNumbers)
{
    namespace fs = std::filesystem;
    std::vector<std::uintmax_t> sizes;
    sizes.reserve(fileNumbers.size());
    for (int n : fileNumbers) {
        std::error_code ec;
        const auto size = fs::file_size(fs::path(directory) / "Out" / (prefix + std::to_string(n) + ".txt"), ec);
        sizes.push_back(ec ? 0 : size);
    }
    return sizes;
}

} // namespace

void loadSimulation(QPromise<SimulationData>& promise,
                    QString directory,
                    std::vector<int> fileNumbers)
{
    const std::string dir = directory.toStdString();

    // La progression est proportionnelle au volume de données lu :
    // les fichiers de grains, beaucoup plus gros, pèsent bien plus que ceux du conteneur.
    const auto grainSizes = fileSizes(dir, "grain_", fileNumbers);
    const auto containerSizes = fileSizes(dir, "container_", fileNumbers);

    std::uintmax_t total = 0;
    for (auto s : grainSizes) total += s;
    for (auto s : containerSizes) total += s;

    // Unité de progression choisie pour rester dans les bornes d'un int
    const std::uintmax_t unit = std::max<std::uintmax_t>(1, total / std::numeric_limits<int>::max() + 1);
    const int maximum = std::max(1, static_cast<int>(total / unit));
    promise.setProgressRange(0, maximum);

    std::uintmax_t bytesRead = 0;
    auto report = [&](std::uintmax_t fileSize) {
        bytesRead += fileSize;
        promise.setProgressValue(static_cast<int>(bytesRead / unit));
        return !promise.isCanceled();
    };

    SimulationData data;

    // 1. Grains
    promise.setProgressValueAndText(0, QObject::tr("Chargement des grains..."));
    bool ok = data.spheres.load(dir, fileNumbers, [&](int k) {
        return report(grainSizes[k - 1]);
    });
    if (!ok)
        return;   // annulé

    // Si un fichier manquait, on ne garde que ceux réellement lus
    fileNumbers.resize(data.spheres.getN());
    data.fileNumbers = fileNumbers;

    // 2. Conteneur
    promise.setProgressValueAndText(static_cast<int>(bytesRead / unit),
                                    QObject::tr("Chargement du conteneur..."));
    ok = data.containers.load(dir, fileNumbers, [&](int k) {
        return report(containerSizes[k - 1]);
    });
    if (!ok)
        return;   // annulé

    promise.setProgressValue(maximum);
    promise.addResult(std::move(data));
}
