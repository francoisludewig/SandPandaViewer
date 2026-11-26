#include "myglwidget.h"

#include </usr/include/GL/glu.h>
#include <QSurfaceFormat>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <iostream>
#include <algorithm>
//#include<windows.h>

#include <QApplication>
#include <QFile>
#include <QFutureWatcher>
#include <QImage>
#include <QProcess>
#include <QMessageBox>
#include <QProgressDialog>
#include <QtConcurrent>

#include "loadrangedialog.h"
#include "Repository/simulationloader.h"

MyGLWidget::MyGLWidget(QWidget *parent) : QOpenGLWidget(parent) {
/*    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
  */
    isMouseDragged = false;
    isDoubleClickMouseDragged = false;
    zoom = 1;
    anglex = 0;
    angley = 0;
}

void MyGLWidget::incrementCurrentfile()
{
    if (sphr.getN() == 0)
        return;
    this->currentFile += this->frameStep;
    this->currentFile = this->currentFile % sphr.getN();
    emit this->updateCurrentfile(displayedFileNumber());
    std::cout << "Emit Current file : " << this->currentFile << std::endl;
    this->update();
}

void MyGLWidget::decrementCurrentfile()
{
    if (sphr.getN() == 0)
        return;
    this->currentFile -= this->frameStep;
    if(this->currentFile < 0)
        this->currentFile = ((this->currentFile % sphr.getN()) + sphr.getN()) % sphr.getN();
    emit this->updateCurrentfile(displayedFileNumber());
    std::cout << "Emit Current file : " << this->currentFile << std::endl;
    this->update();
}

void MyGLWidget::play()
{
    this->animation = true;
    this->update();
}

void MyGLWidget::stop()
{
    this->animation = false;
    this->update();
}

void MyGLWidget::reset()
{
    this->currentFile = 0;
    this->update();
    emit this->updateCurrentfile(displayedFileNumber());
    std::cout << "Emit Current file : " << this->currentFile << std::endl;
}

void MyGLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();
}

void MyGLWidget::drawCylinder(double r)
{
    int lats = 100;
    int longs = 100;
    double h = 3;
    int i, j;
    double x1, y1, z1, x2, y2, z2;
    for(i = 0; i <= lats ; i++) {
        x1 = r*cos(2 * M_PI / lats * i);
        y1 = r*sin(2 * M_PI / lats * i);
        x2 = r*cos(2 * M_PI / lats * (i+1));
        y2 = r*sin(2 * M_PI / lats * (i+1));
        // z
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_STRIP);
        for(j = 0 ; j < longs ; j++) {
            z1 = -h/2 + h/longs * j;
            z2 = -h/2 + h/longs * (j+1);
            glNormal3f(x1, y1, 0);
            glVertex3f(x1, y1, z1);
            glNormal3f(x2, y2, 0);
            glVertex3f(x2, y2, z1);
            glNormal3f(x2, y2, 0);
            glVertex3f(x2, y2, z2);
            glNormal3f(x1, y1, 0);
            glVertex3f(x1, y1, z2);
            glNormal3f(x1, y1, 0);
            glVertex3f(x1, y1, z1);
        }
        glEnd();

        // x
        glColor3f(0.0f, 1.0f, 0.0f);
        glBegin(GL_TRIANGLE_STRIP);
        for(j = 0 ; j < longs ; j++) {
            z1 = -h/2 + h/longs * j;
            z2 = -h/2 + h/longs * (j+1);
            glNormal3f(0, x1, y1);
            glVertex3f(z1, x1, y1);
            glNormal3f(0, x2, y2);
            glVertex3f(z1, x2, y2);
            glNormal3f(0, x2, y2);
            glVertex3f(z2, x2, y2);
            glNormal3f(0, x1, y1);
            glVertex3f(z2, x1, y1);
            glNormal3f(0, x1, y1);
            glVertex3f(z1, x1, y1);
        }
        glEnd();

        // y
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_TRIANGLE_STRIP);
        for(j = 0 ; j < longs ; j++) {
            z1 = -h/2 + h/longs * j;
            z2 = -h/2 + h/longs * (j+1);
            glNormal3f(y1, 0, x1);
            glVertex3f(y1, z1, x1);
            glNormal3f(y2, 0, x2);
            glVertex3f(y2, z1, x2);
            glNormal3f(y2, 0, x2);
            glVertex3f(y2, z2, x2);
            glNormal3f(y1, 0, x1);
            glVertex3f(y1, z2, x1);
            glNormal3f(y1, 0, x1);
            glVertex3f(y1, z1, x1);
        }
        glEnd();
    }
}


void MyGLWidget::paintGL()
{

    prepare();
    std::cout << "zoom : " << zoom << std::endl;
    std::cout << "anglex : " << anglex << std::endl;
    std::cout << "angley : " << angley << std::endl;
    std::cout << "posX : " << posX << std::endl;
    std::cout << "posY : " << posY << std::endl;
    GLfloat lightPosition[4] = {3 , 10 , 3, 1};
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT);
    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glTranslated(posX, -posY, 0);

    glRotatef(-angley,1.0,0.0,0.0);
    glRotatef(-anglex,0.0,1.0,0.0);

    glScalef(zoom,zoom,zoom);


    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    sphr.Draw(currentFile);
    if(drawContainer) {
        plr.Draw(currentFile, isLineContainer);
    }
    if(animation)
       run();
    glFinish();
}


void MyGLWidget::run() {
    if (sphr.getN() == 0)
        return;
    this->currentFile += this->frameStep;
    this->currentFile = this->currentFile % sphr.getN();
    std::cout << "Current file : " << this->currentFile << std::endl;
    this->update();
    emit this->updateCurrentfile(displayedFileNumber());
}

void MyGLWidget::setFrameStep(int step)
{
    this->frameStep = step;
}

int MyGLWidget::displayedFileNumber() const
{
    if (currentFile >= 0 && currentFile < static_cast<int>(fileNumbers.size()))
        return fileNumbers[currentFile];
    return currentFile;
}

int MyGLWidget::frameCount() const
{
    return sphr.getN();
}

bool MyGLWidget::exportCurrentImage(const QString &path)
{
    // grabFramebuffer() relance paintGL() : on coupe l'animation pour ne pas avancer d'une image
    const bool wasAnimating = animation;
    animation = false;
    const QImage image = grabFramebuffer();
    animation = wasAnimating;
    if (wasAnimating)
        update();

    if (image.isNull() || !image.save(path)) {
        QMessageBox::critical(this, tr("Export de l'image"),
                              tr("Impossible d'enregistrer l'image dans %1.").arg(path));
        return false;
    }
    return true;
}

bool MyGLWidget::exportVideo(const QString &path, int fps, int first, int last, int step)
{
    if (exporting || loading || sphr.getN() == 0 || step < 1)
        return false;
    first = std::clamp(first, 0, sphr.getN() - 1);
    last = std::clamp(last, first, sphr.getN() - 1);

    exporting = true;
    const bool wasAnimating = animation;
    animation = false;               // sinon paintGL() appelle run() et avance tout seul
    const int savedFile = currentFile;

    // Taille fixe pour toute la vidéo, dimensions paires (exigé par yuv420p)
    const QImage probe = grabFramebuffer();
    const int w = probe.width() & ~1;
    const int h = probe.height() & ~1;

    auto restore = [&]() {
        currentFile = savedFile;
        animation = wasAnimating;
        exporting = false;
        emit updateCurrentfile(displayedFileNumber());
        update();
    };

    if (w < 2 || h < 2) {
        restore();
        return false;
    }

    QProcess ffmpeg;
    ffmpeg.setProcessChannelMode(QProcess::SeparateChannels);
    ffmpeg.start(QStringLiteral("ffmpeg"), {
        "-y", "-loglevel", "error",
        "-f", "rawvideo", "-pix_fmt", "rgb24",
        "-s", QString("%1x%2").arg(w).arg(h),
        "-r", QString::number(fps),
        "-i", "-",
        "-c:v", "libx264", "-preset", "veryfast", "-crf", "18",
        "-pix_fmt", "yuv420p",
        "-movflags", "+faststart",
        path });
    if (!ffmpeg.waitForStarted()) {
        QMessageBox::critical(this, tr("Export vidéo"),
                              tr("Impossible de lancer ffmpeg. Vérifiez qu'il est installé "
                                 "et accessible dans le PATH."));
        restore();
        return false;
    }

    const int total = (last - first) / step + 1;
    QProgressDialog progress(tr("Export de la vidéo..."), tr("Annuler"), 0, total, window());
    progress.setWindowTitle(tr("Export vidéo"));
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setValue(0);

    bool canceled = false;
    bool pipeError = false;
    int k = 0;
    for (int i = first; i <= last; i += step, ++k) {
        if (progress.wasCanceled()) {
            canceled = true;
            break;
        }
        currentFile = i;
        emit updateCurrentfile(displayedFileNumber());

        QImage image = grabFramebuffer();     // rendu de paintGL() pour l'image i
        if (image.width() != w || image.height() != h)
            image = image.copy(0, 0, w, h);   // tronque la ligne/colonne impaire, ou s'adapte si la fenêtre a changé
        image = image.convertToFormat(QImage::Format_RGB888);

        // Ligne par ligne : les lignes d'un QImage sont alignées sur 4 octets
        for (int y = 0; y < h; ++y) {
            if (ffmpeg.write(reinterpret_cast<const char *>(image.constScanLine(y)), w * 3) < 0) {
                pipeError = true;
                break;
            }
        }
        // Attend que ffmpeg ait consommé l'image (évite d'accumuler des Go en mémoire)
        while (!pipeError && ffmpeg.bytesToWrite() > 0) {
            if (!ffmpeg.waitForBytesWritten(-1))
                pipeError = true;
        }
        if (pipeError || ffmpeg.state() != QProcess::Running) {
            pipeError = true;
            break;
        }

        progress.setValue(k + 1);
        QApplication::processEvents();
    }

    ffmpeg.closeWriteChannel();
    if (canceled)
        ffmpeg.kill();
    ffmpeg.waitForFinished(-1);
    progress.close();

    bool ok = !canceled && !pipeError
              && ffmpeg.exitStatus() == QProcess::NormalExit && ffmpeg.exitCode() == 0;
    if (!ok) {
        QFile::remove(path);
        if (!canceled) {
            const QString err = QString::fromLocal8Bit(ffmpeg.readAllStandardError()).trimmed();
            QMessageBox::critical(this, tr("Export vidéo"),
                                  tr("L'encodage a échoué.") + (err.isEmpty() ? QString() : "\n\n" + err));
        }
    }

    restore();
    return ok;
}

void MyGLWidget::loadDirectory(QString directory)
{
    if (exporting)
        return;
    if (loading)
        return;

    // 1. Détection des fichiers disponibles (rapide : on teste seulement leur existence)
    QApplication::setOverrideCursor(Qt::WaitCursor);
    const int fileCount = SphereRepository::countFiles(directory.toStdString());
    QApplication::restoreOverrideCursor();

    if (fileCount == 0) {
        QMessageBox::warning(this, tr("Aucun fichier"),
                             tr("Aucun fichier grain_0.txt trouvé dans %1/Out.").arg(directory));
        return;
    }

    // 2. Choix du premier, du dernier fichier et du pas
    LoadRangeDialog rangeDialog(fileCount, window());
    if (rangeDialog.exec() != QDialog::Accepted)
        return;
    const std::vector<int> selection = rangeDialog.selectedFileNumbers();

    // 3. Chargement dans un thread secondaire, avec fenêtre de progression
    loading = true;

    // Plage 0..0 = indicateur d'activité, en attendant la plage réelle fixée par le thread
    auto *progress = new QProgressDialog(tr("Chargement..."), tr("Annuler"), 0, 0, window());
    progress->setWindowTitle(tr("Chargement de la simulation"));
    progress->setWindowModality(Qt::WindowModal);
    progress->setMinimumDuration(0);
    progress->setAutoClose(false);
    progress->setAutoReset(false);
    progress->setValue(0);

    auto *watcher = new QFutureWatcher<SimulationData>(this);
    connect(watcher, &QFutureWatcher<SimulationData>::progressRangeChanged,
            progress, &QProgressDialog::setRange);
    connect(watcher, &QFutureWatcher<SimulationData>::progressValueChanged,
            progress, &QProgressDialog::setValue);
    connect(watcher, &QFutureWatcher<SimulationData>::progressTextChanged,
            progress, &QProgressDialog::setLabelText);
    connect(progress, &QProgressDialog::canceled,
            watcher, &QFutureWatcher<SimulationData>::cancel);

    connect(watcher, &QFutureWatcher<SimulationData>::finished, this, [this, watcher, progress]() {
        QFuture<SimulationData> future = watcher->future();
        if (!future.isCanceled() && future.resultCount() > 0) {
            // De retour dans le thread graphique : on remplace les données d'un bloc
            SimulationData data = future.takeResult();
            sphr = std::move(data.spheres);
            plr = std::move(data.containers);
            fileNumbers = std::move(data.fileNumbers);
            reset();
        }
        progress->close();
        progress->deleteLater();
        watcher->deleteLater();
        loading = false;
    });

    watcher->setFuture(QtConcurrent::run(loadSimulation, directory, selection));
}

void MyGLWidget::on_new_container_visual_parameter(QString directory)
{
    if(directory.toStdString().compare("Full Container") == 0) {
        drawContainer = true;
        isLineContainer = false;
    } else if(directory.toStdString().compare("Line Container") == 0) {
        drawContainer = true;
        isLineContainer = true;
    } else {
        drawContainer = false;
    }
    update();
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    mousePosX = event->pos().x();
    mousePosY = event->pos().y();
    if(event->button() == Qt::MouseButton::LeftButton)
        isMouseDragged = true;
    std::cout << "Down on ("<< event->pos().x() << ", "<< event->pos().y() << ")\n" << std::endl;
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        isMouseDragged = false;
        isDoubleClickMouseDragged = false;
    }
    if(event->button() == Qt::MouseButton::MiddleButton) {
        zoom = 1;
        anglex = 0;
        angley = 0;
        posX = 0;
        posY = 0;
        update();
    }
    std::cout << "Release on ("<< event->pos().x() << ", "<< event->pos().y() << ")\n" << std::endl;
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDragged) {
        double dx =  (double)(event->pos().x()-mousePosX)/5;
        double dy =  (double)(event->pos().y()-mousePosY)/5;

        std::cout << "Move displacement ("<< event->pos().x()-mousePosX << ", "<< event->pos().y()-mousePosY << ")\n" << std::endl;
        mousePosX = event->pos().x();
        mousePosY = event->pos().y();

        anglex += dx;
        if(anglex > 360)
            anglex -= 360;
        if(anglex < 0)
            anglex += 360;
        angley += dy;
        update();
    } else if (isDoubleClickMouseDragged) {
        double dx =  (double)(event->pos().x()-mousePosX)/100;
        double dy =  (double)(event->pos().y()-mousePosY)/100;
        std::cout << "d ("<< dx << ", "<< dy << ")\n" << std::endl;
        mousePosX = event->pos().x();
        mousePosY = event->pos().y();
        posX += dx;
        posY += dy;
        update();
    }
}

void MyGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    std::cout << "Double click" << std::endl;
    isDoubleClickMouseDragged = true;
    mousePosX = event->pos().x();
    mousePosY = event->pos().y();
}

void MyGLWidget::wheelEvent(QWheelEvent *event)
{
    std::cout << "Wheel rolls " << event->angleDelta().ry() << std::endl;
    double D = event->angleDelta().ry();
    if(D > 0){
        zoom = zoom*(fabs(D/600)+1);
    }
    if(D < 0){
        zoom = zoom/(fabs(D/600)+1);
    }
    update();
}

void MyGLWidget::prepare()
{
    GLfloat mat_specular[4] = { (GLfloat)(0.5), (GLfloat)(0.5), (GLfloat)(0.5), (GLfloat)(1.0) };
    GLfloat light_ambient[4] = {(GLfloat)(0.3), (GLfloat)(0.3),(GLfloat)(0.3), (GLfloat)(1.0) };

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_specular);

    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    glMaterialf(GL_FRONT, GL_SHININESS, 100);


    glHint(GL_FOG_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,.0f);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,.0f);
}

