#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Forcer la locale C (séparateur = point)
    std::locale("C");
    MainWindow w;
    w.show();
    return a.exec();
}
