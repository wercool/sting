#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanning = false;
    thrust    = {};
    thrustGraphicsViewScene = new QGraphicsScene;
    drawTimer = new QTimer(this);
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(drawGraph()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    try
    {
        controlDevice = new serial::Serial(ui->controlDeviceLineEdit->text().toStdString(), 115200, serial::Timeout::simpleTimeout(500));
        if(controlDevice->isOpen())
        {
            controlDevice->write("STOP");
            controlDevice->flush();
            controlDevice->write("GETID");
            std::string result = controlDevice->readline();

            QMessageBox msgBox;
            msgBox.setText(result.c_str());
            msgBox.exec();
        }
    }
    catch (serial::IOException ex)
    {
        qDebug("%s", ex.what());
    }


}

void MainWindow::on_startButton_clicked()
{
    scanning = true;
    thrustGraphicsViewScene->setSceneRect(0, 0, ui->thrustGraphicsView->width() - 10, ui->thrustGraphicsView->height() - 10);
    ui->thrustGraphicsView->setScene(thrustGraphicsViewScene);
    drawTimer->start(10);

    controlDevice->write("START");
    new std::thread(&MainWindow::readForce, this);
}

void MainWindow::on_stopButton_clicked()
{
    controlDevice->write("STOP");
    scanning = false;
}

void MainWindow::drawGraph()
{
    thrustGraphicsViewScene->clear();
    float shift = ui->thrustGraphicsView->height() - 20;
    float _x = 0;
    float _y = ui->thrustGraphicsView->height() - 20;
    for(auto const& val: thrust)
    {
        float _val = val * 100;
        thrustGraphicsViewScene->addLine(_x, _y, (_x + 0.1), shift - _val);
        _x += 0.1;
        _y = shift - _val;
    }
}

void MainWindow::readForce()
{
    while (scanning)
    {
        std::string response = controlDevice->readline();
        if (response.find("T:") != std::string::npos)
        {
            int value_str_pos = response.find_first_of(":") + 1;
            std::string value_str = response.substr(value_str_pos);
            int value = atoi(value_str.c_str());
            qDebug("%d", value);
            thrust.push_back(value);

//            if (value > 1)
//            {
//                thrust.push_back(value);
//            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
