#ifndef SIMULATIONLOADER_H
#define SIMULATIONLOADER_H

#include <QPromise>
#include <QString>
#include <vector>

#include "Repository/sphererepository.h"
#include "Repository/containerrepository.h"

// Données d'une simulation chargée hors du thread graphique,
// puis transférées d'un bloc vers MyGLWidget.
struct SimulationData
{
    SphereRepository spheres;
    ContainerRepository containers;
    std::vector<int> fileNumbers;   // numéros des fichiers réellement chargés
};

// Fonction exécutée dans un thread secondaire via QtConcurrent::run.
// Progression proportionnelle à la taille des fichiers lus (grains, puis conteneur).
// Le chargement s'arrête proprement si le QFuture associé est annulé.
// Ne touche à aucun objet graphique ni à OpenGL.
void loadSimulation(QPromise<SimulationData>& promise,
                    QString directory,
                    std::vector<int> fileNumbers);

#endif // SIMULATIONLOADER_H
