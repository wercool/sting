#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>     // std::cin, std::cout
#include <fstream>      // std::ifstream

#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <QBrush>

MainWindow* MainWindow::pMainWindow = NULL;
bool MainWindow::instanceFlag = false;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scanning = false;
    thrust    = {};
    _x = 0;
    dx = 0.5;
    thrustGraphicsViewScene = new QGraphicsScene;
    drawTimer = new QTimer(this);
    connect(drawTimer, SIGNAL(timeout()), this, SLOT(drawGraph()));
    testMode = true;

    drawGraph();
}

MainWindow *MainWindow::getInstance()
{
    if(!instanceFlag)
    {
        pMainWindow = new MainWindow();
        instanceFlag = true;
        return pMainWindow;
    }
    else
    {
        return pMainWindow;
    }
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
    testMode = true;
    scanning = true;
    thrustGraphicsViewScene->setSceneRect(0, 0, ui->thrustGraphicsView->width() - 10, ui->thrustGraphicsView->height() - 10);
    ui->thrustGraphicsView->setScene(thrustGraphicsViewScene);
    drawTimer->start(1);

    controlDevice->write("START");
    new std::thread(&MainWindow::readForce, this);
}

void MainWindow::on_stopButton_clicked()
{
    controlDevice->write("STOP");
    scanning = false;
    thrustGraphicsViewScene->clear();
    _x = 0;
    thrust.clear();
}

void MainWindow::drawGraph()
{
    thrustGraphicsViewScene->clear();
    thrustGraphicsViewScene->setSceneRect(0, 0, ((double)thrust.size() / 2) / (0.5 / dx), ui->thrustGraphicsView->height() - 10);

    timeLineStart = new TimeLine();
    thrustGraphicsViewScene->addItem(timeLineStart);

    timeLineEnd = new TimeLine();
    thrustGraphicsViewScene->addItem(timeLineEnd);

    ui->thrustGraphicsView->setScene(thrustGraphicsViewScene);

    timeLineStart->setPen(QPen(Qt::darkGreen, 1.0, Qt::DashLine));
    timeLineStart->setLine(100, 0, 100, ui->thrustGraphicsView->height());

    timeLineEnd->setPen(QPen(Qt::darkRed, 1.0, Qt::DashLine));
    timeLineEnd->setLine(200, 0, 200, ui->thrustGraphicsView->height());

    QBrush brush;
    timeLineStartHandler = new TimeLineHandler(timeLineStart);
    timeLineStartHandler->start = true;
    timeLineStartHandler->setRect(0, 0, 12, 12);
    timeLineStartHandler->setPos(94, ui->thrustGraphicsView->height() / 2 - 6);
    brush = timeLineStartHandler->brush();
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::green);
    timeLineStartHandler->setBrush(brush);
    thrustGraphicsViewScene->addItem(timeLineStartHandler);

    timeLineEndHandler = new TimeLineHandler(timeLineEnd);
    timeLineEndHandler->start = false;
    timeLineEndHandler->setRect(0, 0, 12, 12);
    timeLineEndHandler->setPos(194, ui->thrustGraphicsView->height() / 2 - 6);
    brush = timeLineStartHandler->brush();
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);
    timeLineEndHandler->setBrush(brush);
    thrustGraphicsViewScene->addItem(timeLineEndHandler);

    timeLineStartText = new QGraphicsTextItem;
    timeLineStartText->setPlainText(".....");
    timeLineStartText->setPos(timeLineStartHandler->pos().x() + 12, timeLineStartHandler->pos().y() - 20);
    thrustGraphicsViewScene->addItem(timeLineStartText);

    timeLineEndText   = new QGraphicsTextItem;
    timeLineEndText->setPlainText(".....");
    timeLineEndText->setPos(timeLineEndHandler->pos().x() + 12, timeLineEndHandler->pos().y() - 20);
    thrustGraphicsViewScene->addItem(timeLineEndText);



    forceLine = new QGraphicsLineItem;
    forceLine->setPen(QPen(Qt::darkBlue, 1.0, Qt::DashLine));
    forceLine->setLine(0, ui->thrustGraphicsView->height() / 2, ui->thrustGraphicsView->width(), ui->thrustGraphicsView->height() / 2);
    thrustGraphicsViewScene->addItem(forceLine);

    forceLineHandler = new ForceLineHandler(forceLine);
    forceLineHandler->setRect(0, 0, 12, 12);
    forceLineHandler->setPos(50, ui->thrustGraphicsView->height() / 2 - 6);
    brush = forceLineHandler->brush();
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::blue);
    forceLineHandler->setBrush(brush);
    thrustGraphicsViewScene->addItem(forceLineHandler);

    forceLineText   = new QGraphicsTextItem;
    forceLineText->setPlainText(".....");
    forceLineText->setPos(forceLineHandler->pos().x(), forceLineHandler->pos().y() - 20);
    thrustGraphicsViewScene->addItem(forceLineText);


    float shift = ui->thrustGraphicsView->height() - 10;
    float _x = 0;
    float _y = ui->thrustGraphicsView->height() - 10;
    for(auto const& val: thrust)
    {
        float _val = (float)val;
        float dy = (_val / 1023) * (ui->thrustGraphicsView->height() - 20);
        thrustGraphicsViewScene->addLine(_x, _y, (_x + dx), shift - dy);
        _x += dx;
        _y = shift - dy;
    }
    if (testMode)
    {
        if (thrust.size() > (unsigned int)ui->thrustGraphicsView->width())
        {
            thrust.clear();
        }
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
        }
        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
}

void MainWindow::on_loadFromFileButton_clicked()
{
    QFileDialog dialog(this);
    if (dialog.exec())
    {
        testMode = false;
        thrust.clear();
        QStringList fileNames = dialog.selectedFiles();
        QString fileName = fileNames.at(0);
        qDebug("%s", fileName.toStdString().c_str());

        std::ifstream inputStream(fileName.toStdString());
        std::string line;
        int i = 0;
        while (getline(inputStream, line, '\n'))
        {
            if (i++ > 28500 && i < 32000)
            {
                if (line.length() == 4)
                {
                    int value = std::atoi(line.c_str());
                    qDebug("%d", value);
                    thrust.push_back(value);
                }
            }
        }
        inputStream.close();
        drawGraph();
    }
}

void MainWindow::MainWindow::resizeEvent(QResizeEvent *event)
{
    (void)event;
    drawGraph();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    dx = 1 / (double)value;
    drawGraph();
}
