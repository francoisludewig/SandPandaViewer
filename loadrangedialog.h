#ifndef LOADRANGEDIALOG_H
#define LOADRANGEDIALOG_H

#include <QDialog>
#include <vector>

class QSpinBox;
class QLabel;

// Fenêtre de choix des fichiers à charger : premier, dernier et pas.
class LoadRangeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoadRangeDialog(int fileCount, QWidget *parent = nullptr);

    // Numéros des fichiers sélectionnés : first, first+step, ... <= last
    std::vector<int> selectedFileNumbers() const;

private:
    QSpinBox *spinFirst;
    QSpinBox *spinLast;
    QSpinBox *spinStep;
    QLabel *summary;

    void updateSummary();
};

#endif // LOADRANGEDIALOG_H
