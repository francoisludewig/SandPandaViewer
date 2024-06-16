#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->openGLWidget, &MyGLWidget::updateCurrentfile, this, &MainWindow::on_currentfile_changed);
    connect(this, &MainWindow::on_new_directory, ui->openGLWidget, &MyGLWidget::loadDirectory);
    connect(this, &MainWindow::on_new_container_visual, ui->openGLWidget, &MyGLWidget::on_new_container_visual_parameter);

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
        emit on_new_directory(f.toStdString());
        break;
    }

}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    emit this->on_new_container_visual(arg1.toStdString());
}

