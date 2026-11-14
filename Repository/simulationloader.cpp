#include "Repository/simulationloader.h"

#include <QObject>

void loadSimulation(QPromise<SimulationData>& promise,
                    QString directory,
                    std::vector<int> fileNumbers)
{
    const int M = static_cast<int>(fileNumbers.size());
    const std::string dir = directory.toStdString();

    promise.setProgressRange(0, 2 * M);

    SimulationData data;

    // 1. Grains
    promise.setProgressValueAndText(0, QObject::tr("Chargement des grains..."));
    bool ok = data.spheres.load(dir, fileNumbers, [&promise](int k) {
        promise.setProgressValue(k);
        return !promise.isCanceled();
    });
    if (!ok)
        return;   // annulé

    // Si un fichier manquait, on ne garde que ceux réellement lus
    fileNumbers.resize(data.spheres.getN());
    data.fileNumbers = fileNumbers;

    // 2. Conteneur
    promise.setProgressValueAndText(M, QObject::tr("Chargement du conteneur..."));
    ok = data.containers.load(dir, fileNumbers, [&promise, M](int k) {
        promise.setProgressValue(M + k);
        return !promise.isCanceled();
    });
    if (!ok)
        return;   // annulé

    promise.setProgressValue(2 * M);
    promise.addResult(std::move(data));
}
