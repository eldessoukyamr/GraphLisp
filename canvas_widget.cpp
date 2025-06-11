#include "canvas_widget.hpp"

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QResizeEvent>
#include <QLayout>

/* Constructor that creates the canvas widget by creating a QGraphicsView and attaches a QGraphicsScene to it*/
CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {
    // TODO: your code here...

    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);

    view->setScene(scene);
}

/* Adds a QGraphicsItem to the current scene */
void CanvasWidget::addGraphic(QGraphicsItem *item) {
    scene->addItem(item);
}


/* Resizes the drawing canvas as appropiate */
void CanvasWidget::resizeEvent(QResizeEvent* event)
{
    const auto& newSize = event->size();

    view->setMinimumSize(800, 400);
}