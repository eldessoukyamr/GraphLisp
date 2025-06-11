#ifndef CANVAS_WIDGET_HPP
#define CANVAS_WIDGET_HPP

#include <QWidget>

class QGraphicsItem;
class QGraphicsView;
class QGraphicsScene;

class CanvasWidget : public QWidget {
Q_OBJECT

public:

    CanvasWidget(QWidget *parent = nullptr);

public slots:

    void addGraphic(QGraphicsItem *item);
protected:
    void resizeEvent(QResizeEvent* event) override;
private:
    QGraphicsView* view;
    QGraphicsScene *scene;
};


#endif
