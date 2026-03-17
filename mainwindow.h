#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonPlus_clicked();
    void on_pushButtonMinus_clicked();
    void on_pushButtonPlay_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonReset_clicked();
    void on_currentfile_changed(int);
    void on_pushButton_2_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_spinBoxStep_valueChanged(int value);

signals:
    void new_directory(QString directory);
    void new_container_visual(QString directory);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
