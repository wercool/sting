#ifndef FORCELINEHANDLER_H
#define FORCELINEHANDLER_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

class ForceLineHandler : public QGraphicsEllipseItem
{
public:
    ForceLineHandler(QGraphicsLineItem *forceLine);
    QGraphicsLineItem *forceLine;

    // QGraphicsItem interface
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
};

#endif // FORCELINEHANDLER_H
