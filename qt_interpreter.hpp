#ifndef QT_INTERPRETER_HPP
#define QT_INTERPRETER_HPP

#include <string>

#include <QObject>
#include <QString>
#include <QGraphicsItem>


#include "interpreter.hpp"

class QtInterpreter : public QObject, private Interpreter {
Q_OBJECT

public:

    QtInterpreter(QObject *parent = nullptr);
private:
    Expression eval_misc(const Expression& expr) override;

    void draw(const Expression& expr);
signals:

    void drawGraphic(QGraphicsItem *item);

    void info(QString message);

    void error(QString message);


public slots:

    void parseAndEvaluate(QString entry);
};

#endif
