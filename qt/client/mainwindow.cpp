#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    new std::thread(&MainWindow::readForce, this);
    try
    {
        serial::Serial controlDevice(ui->controlDeviceLineEdit->text().toStdString(), 115200, serial::Timeout::simpleTimeout(500));
        if(controlDevice.isOpen())
        {
            controlDevice.flush();
            controlDevice.write("GETID");
            std::string result = controlDevice.readline();


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

}

void MainWindow::on_stopButton_clicked()
{

}

void MainWindow::readForce()
{
    while (true)
    {
        qDebug("!!!!!!!");
    }
}
