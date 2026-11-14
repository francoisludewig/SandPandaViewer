#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);

    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);        // pas OpenGL ES
    format.setProfile(QSurfaceFormat::CompatibilityProfile); // pipeline fixe
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    // Forcer la locale C (séparateur = point)
    std::locale("C");
    MainWindow w;
    w.show();
    return a.exec();
}
