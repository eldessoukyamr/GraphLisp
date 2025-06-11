#include "qgraphics_arc_item.hpp"

#include <cmath>

#include <QDebug>
#include <QPainter>
#include <QGraphicsEllipseItem>

/* Constructor for a custom arc drawing QGraphicsItem that is inherits QGraphicsEllipseItem */
QGraphicsArcItem::QGraphicsArcItem(qreal x, qreal y, qreal width, qreal height,
                                   QGraphicsItem *parent) :  QGraphicsEllipseItem(x, y, width, height, parent){
    QPen drawPen(Qt::black, 1);
    setPen(drawPen);
}

/* Overloaded drawing function to draw the arc correctly */
void QGraphicsArcItem::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget) {
    painter->setPen(pen());

    painter->drawArc(rect(), startAngle(), spanAngle());
}

