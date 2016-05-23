#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serial/include/serial/serial.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    serial::Serial controlDevice;
    ~MainWindow();

private slots:
    void on_connectButton_clicked();

    void on_startButton_clicked();

    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
    void readForce();
};

#endif // MAINWINDOW_H
