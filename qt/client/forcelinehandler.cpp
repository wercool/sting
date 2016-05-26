#include "mainwindow.h"
#include "forcelinehandler.h"

ForceLineHandler::ForceLineHandler(QGraphicsLineItem *forceLine)
{
    this->forceLine = forceLine;
}

void ForceLineHandler::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    (void)event;
}

void ForceLineHandler::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    this->setPos(event->scenePos().x() - 6, event->scenePos().y() - 6);
    this->forceLine->setLine(0, event->scenePos().y(), MainWindow::getInstance()->thrustGraphicsViewScene->width(), event->scenePos().y());
    float _val = (860 - 1023 * (((event->scenePos().y() - 20)) / (MainWindow::getInstance()->thrustGraphicsViewScene->height() - 10))) * 5;
    _val += exp(_val / 820) * 60;
    MainWindow::getInstance()->forceLineText->setPlainText(MainWindow::format_string("%.0f g", _val).c_str());
    MainWindow::getInstance()->forceLineText->setPos(event->scenePos().x(), event->scenePos().y() - 20);
}
