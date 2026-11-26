#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

#include "exportvideodialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, &MainWindow::new_directory, ui->openGLWidget, &MyGLWidget::loadDirectory);
    connect(this, &MainWindow::new_container_visual, ui->openGLWidget, &MyGLWidget::on_new_container_visual_parameter);
    connect(ui->openGLWidget, &MyGLWidget::updateCurrentfile, this, &MainWindow::onCurrentFileChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonPlus_clicked()
{
    ui->openGLWidget->incrementCurrentfile();
}


void MainWindow::on_pushButtonMinus_clicked()
{
    ui->openGLWidget->decrementCurrentfile();
}


void MainWindow::on_pushButtonPlay_clicked()
{
    ui->openGLWidget->play();
}


void MainWindow::on_pushButtonStop_clicked()
{
    ui->openGLWidget->stop();
}


void MainWindow::on_pushButtonReset_clicked()
{
    ui->openGLWidget->reset();
}

void MainWindow::onCurrentFileChanged(int currentfile)
{
    std::cout << "Slot on_currentfile_changed" << std::endl;
    ui->currentFile->setText(QString::number(currentfile));
}


void MainWindow::on_pushButton_2_clicked()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::Directory);
    fileDialog.setOption(QFileDialog::DontUseNativeDialog, true);
    int nMode = fileDialog.exec();
    QStringList _fnames = fileDialog.selectedFiles();
    for(auto f : _fnames) {
        emit new_directory(f);
        break;
    }

}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    emit this->new_container_visual(arg1);
}

void MainWindow::on_spinBoxStep_valueChanged(int value)
{
    ui->openGLWidget->setFrameStep(value);
}


void MainWindow::on_pushButtonExportImage_clicked()
{
    auto *gl = ui->openGLWidget;
    if (gl->frameCount() == 0) {
        QMessageBox::information(this, tr("Export de l'image"), tr("Aucune simulation chargée."));
        return;
    }

    const QString defaultName = QString("grain_%1.png").arg(gl->displayedFileNumber());
    QString path = QFileDialog::getSaveFileName(this, tr("Enregistrer l'image"), defaultName,
                                                tr("Image PNG (*.png);;Image JPEG (*.jpg *.jpeg);;Image BMP (*.bmp)"),
                                                nullptr, QFileDialog::DontUseNativeDialog);
    if (path.isEmpty())
        return;
    if (QFileInfo(path).suffix().isEmpty())
        path += ".png";

    gl->exportCurrentImage(path);
}

void MainWindow::on_pushButtonExportVideo_clicked()
{
    auto *gl = ui->openGLWidget;
    if (gl->frameCount() == 0) {
        QMessageBox::information(this, tr("Export vidéo"), tr("Aucune simulation chargée."));
        return;
    }

    ExportVideoDialog dialog(gl->frameCount(), gl->loadedFileNumbers(), this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString path = QFileDialog::getSaveFileName(this, tr("Enregistrer la vidéo"), "simulation.mp4",
                                                tr("Vidéo MP4 (*.mp4)"),
                                                nullptr, QFileDialog::DontUseNativeDialog);
    if (path.isEmpty())
        return;
    if (QFileInfo(path).suffix().isEmpty())
        path += ".mp4";

    if (gl->exportVideo(path, dialog.fps(), dialog.first(), dialog.last(), dialog.step()))
        QMessageBox::information(this, tr("Export vidéo"),
                                 tr("Vidéo enregistrée :\n%1").arg(path));
}
