#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include "serial/include/serial/serial.h"
#include <thread>
#include <stdlib.h>
#include <string.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    serial::Serial *controlDevice;
    QGraphicsScene *thrustGraphicsViewScene;
    QTimer *drawTimer;
    std::vector<int> thrust;
    float _x;
    bool testMode;
    ~MainWindow();

public slots:
    void on_connectButton_clicked();

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void drawGraph();

private slots:

    void on_loadFromFileButton_clicked();
    void resizeEvent(QResizeEvent* event);

private:
    Ui::MainWindow *ui;
    void readForce();

    bool scanning;
};

#endif // MAINWINDOW_H
