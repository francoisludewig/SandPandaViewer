#ifndef EXPORTVIDEODIALOG_H
#define EXPORTVIDEODIALOG_H

#include <QDialog>
#include <vector>

class QSpinBox;
class QLabel;

// Fenêtre de paramètres de l'export vidéo : images par seconde,
// première image, dernière image et pas entre les images.
// Les indices portent sur les fichiers chargés (0 .. frameCount-1) ;
// fileNumbers sert à afficher les numéros grain_n correspondants.
class ExportVideoDialog : public QDialog
{
    Q_OBJECT
public:
    ExportVideoDialog(int frameCount, const std::vector<int> &fileNumbers,
                      QWidget *parent = nullptr);

    int fps() const;
    int first() const;
    int last() const;
    int step() const;

    // Nombre d'images qui seront exportées
    int imageCount() const;

private:
    QSpinBox *spinFps;
    QSpinBox *spinFirst;
    QSpinBox *spinLast;
    QSpinBox *spinStep;
    QLabel *summary;
    std::vector<int> numbers;

    int fileNumber(int index) const;
    void updateSummary();
};

#endif // EXPORTVIDEODIALOG_H
