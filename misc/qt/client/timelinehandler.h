#ifndef TIMELINEHANDLER_H
#define TIMELINEHANDLER_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

#include "timeline.h"

class TimeLineHandler : public QGraphicsEllipseItem
{
public:
    TimeLineHandler(TimeLine *timeLine);
    TimeLine *timeLine;
    bool start;

    // QGraphicsItem interface
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // TIMELINEHANDLER_H
