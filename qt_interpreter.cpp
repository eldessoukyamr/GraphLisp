#include "qt_interpreter.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>

#include <QBrush>
#include <QDebug>
#include <QtMath>
#include <QPen>

#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>

#include "qgraphics_arc_item.hpp"

#include "interpreter_semantic_error.hpp"


QtInterpreter::QtInterpreter(QObject *parent) : QObject(parent) {
    Interpreter();
}

/* Parses and evaluates an inputted QString program */
void QtInterpreter::parseAndEvaluate(QString entry) {
    std::stringstream stream(entry.toStdString());
    std::stringstream out_stream;
    if (!parse(stream))
    {
        emit error("Unable to parse");
    }

    try
    {
        out_stream << eval();
    }
    catch (...)
    {
        emit error("Test");

        return;
    }

    for (const auto& expr : graphics)
    {
        draw(expr);
    }

    emit info(QString::fromStdString(out_stream.str()));
}

/* Draw function that creates a QGraphicsItem based on the Expression given, and sends a signal to the canvas with this item */
void QtInterpreter::draw(const Expression& expr)
{
    QGraphicsItem* obj = nullptr;
    switch (expr.head.type)
    {
        case PointType: {
            QGraphicsEllipseItem* point = new QGraphicsEllipseItem(expr.head.value.point_value.x - 2.5, expr.head.value.point_value.y - 2.5, 5, 5);
            point->setBrush(Qt::black);
            obj = point;
            break;
        }
        case LineType: {
            obj = new QGraphicsLineItem(expr.head.value.line_value.first.x,
                                        expr.head.value.line_value.first.y,
                                        expr.head.value.line_value.second.x,
                                        expr.head.value.line_value.second.y);
            break;
        }
        case ArcType: {
            const Point* cen = &expr.head.value.arc_value.center;
            const Point* s = &expr.head.value.arc_value.start;
            Number span = expr.head.value.arc_value.span;

            Number x = (s->x - cen->x);
            Number y = (cen->y - s->y);


            Number radius = qSqrt((x * x) + (y * y));
            Number diam = 2.0 * radius;
            
            QGraphicsArcItem* item = new QGraphicsArcItem(cen->x - radius, cen->y - radius, diam, diam);
            item->setStartAngle(qRadiansToDegrees(qAtan2(y, x)) * 16.0);
            item->setSpanAngle(qRadiansToDegrees(span) * 16.0);

            obj = item;

            break;
        }
        case RectType: { //fix
            const Rect* rect = &expr.head.value.rect_value;
            obj = new QGraphicsRectItem(rect->x2,
                                        rect->y2,
                                        rect->x1 - rect->x2,
                                        rect->y1 - rect->y2);
            break;
        }
        case FillRectType: {
            const FillRect* fill_rect = &expr.head.value.fillRect_value;
            const Rect* rect = &fill_rect->rect;
            QGraphicsRectItem* item = new QGraphicsRectItem(rect->x2,
                                        rect->y2,
                                        rect->x1 - rect->x2,
                                        rect->y1 - rect->y2);

            item->setBrush(QColor(fill_rect->r, fill_rect->g, fill_rect->b));
            item->setPen(QPen(Qt::NoPen));
            obj = item;
            break;
        }
        case EllipseType: {
            const Rect* rect = &expr.head.value.ellipse_value.rect;

            obj = new QGraphicsEllipseItem(rect->x1,
                                           rect->y1,
                                           rect->x2 - rect->x1,
                                           rect->y2 - rect->y1);



            break;
        }
        default: {
            throw InterpreterSemanticError("Invalid expression for drawing");
            break;
        }
    }


    emit drawGraphic(obj);
}

/* Overloaded eval_misc function to handle draw */
Expression QtInterpreter::eval_misc(const Expression& expr)
{
    graphics.empty();
    if (expr.head.type == SymbolType)
    {
        std::string op = expr.head.value.sym_value;
        const std::vector<Expression>& tail = expr.tail;

        if (op == "draw")
        {
            if (tail.empty())
            {
                throw InterpreterSemanticError("Draw expects at least one expression");
            }

            for (const auto &sub_expr : tail)
            {
                Expression elem = eval_expression(sub_expr);
                graphics.push_back(elem);
                //draw(elem);
            }
        }
        else
        {
            throw InterpreterSemanticError("Unknown symbol: " + expr.head.value.sym_value);
        }
    }
    else
    {
        throw InterpreterSemanticError("Unknown symbol: " + expr.head.value.sym_value);
    }
    return Expression();
}