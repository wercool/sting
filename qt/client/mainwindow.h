#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include "serial/include/serial/serial.h"
#include <thread>
#include <stdlib.h>
#include <string.h>

#include "timeline.h"
#include "timelinehandler.h"
#include "forcelinehandler.h"


namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    static MainWindow *getInstance();

    serial::Serial *controlDevice;
    QGraphicsScene *thrustGraphicsViewScene;
    QTimer *drawTimer;
    std::vector<int> thrust;
    float _x;
    float dx;
    bool testMode;
    TimeLine *timeLineStart;
    TimeLine *timeLineEnd;
    TimeLineHandler *timeLineStartHandler;
    TimeLineHandler *timeLineEndHandler;

    QGraphicsTextItem *timeLineStartText;
    QGraphicsTextItem *timeLineEndText;

    QGraphicsLineItem *forceLine;

    ForceLineHandler *forceLineHandler;
    QGraphicsTextItem *forceLineText;

    ~MainWindow();

    Ui::MainWindow *ui;


    static std::string format_string(const std::string fmt_str, ...)
    {
        int final_n, n = ((int)fmt_str.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
        std::string str;
        std::unique_ptr<char[]> formatted;
        va_list ap;
        while(1)
        {
            formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
            strcpy(&formatted[0], fmt_str.c_str());
            va_start(ap, fmt_str);
            final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
            va_end(ap);
            if (final_n < 0 || final_n >= n)
                n += abs(final_n - n + 1);
            else
                break;
        }
        return std::string(formatted.get());
    }
public slots:
    void on_connectButton_clicked();

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void drawGraph();



private slots:

    void on_loadFromFileButton_clicked();
    void resizeEvent(QResizeEvent* event);

    void on_horizontalSlider_valueChanged(int value);

private:

    static MainWindow* pMainWindow;
    static bool instanceFlag;

    void readForce();

    bool scanning;
};

#endif // MAINWINDOW_H
