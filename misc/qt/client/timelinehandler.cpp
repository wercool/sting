#include "mainwindow.h"
#include "timelinehandler.h"

TimeLineHandler::TimeLineHandler(TimeLine *timeLine)
{
    this->timeLine = timeLine;
}

void TimeLineHandler::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(event->scenePos().x() - 6, event->scenePos().y() - 6);
    this->timeLine->setLine(event->scenePos().x(), 0, event->scenePos().x(), MainWindow::getInstance()->thrustGraphicsViewScene->height());
    if (start)
    {
        MainWindow::getInstance()->timeLineStartText->setPlainText(MainWindow::format_string("%.0f ms", event->scenePos().x() * (1 / MainWindow::getInstance()->dx)).c_str());
        MainWindow::getInstance()->timeLineStartText->setPos(event->scenePos().x() + 12, event->scenePos().y() - 20);
    }
    else
    {
        MainWindow::getInstance()->timeLineEndText->setPlainText(MainWindow::format_string("%.0f ms", event->scenePos().x() * (1 / MainWindow::getInstance()->dx)).c_str());
        MainWindow::getInstance()->timeLineEndText->setPos(event->scenePos().x() + 12, event->scenePos().y() - 20);
    }
}

void TimeLineHandler::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    (void)event;
}
