#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, &MainWindow::new_directory, ui->openGLWidget, &MyGLWidget::loadDirectory);
    connect(this, &MainWindow::new_container_visual, ui->openGLWidget, &MyGLWidget::on_new_container_visual_parameter);
    connect(ui->openGLWidget, &MyGLWidget::updateCurrentfile, this, &MainWindow::on_currentfile_changed);
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

void MainWindow::on_currentfile_changed(int currentfile)
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

