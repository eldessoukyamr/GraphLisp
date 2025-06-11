#include "main_window.hpp"

#include <iostream>

#include <QLayout>
#include <QVBoxLayout>
#include <QShowEvent>
#include <QTextStream>
#include <QFile>

#include "message_widget.hpp"
#include "canvas_widget.hpp"
#include "repl_widget.hpp"
#include "interpreter_semantic_error.hpp"

MainWindow::MainWindow(QWidget *parent) : MainWindow("", parent) {
}

/* Constructor to create the QT windows the user sees.
   Incorporates a input box, drawing canvas, and info message box */
MainWindow::MainWindow(std::string filename, QWidget *parent) : QWidget(parent) {
    file = filename;

    layout = new QVBoxLayout(this);
    message = new MessageWidget(this);
    canvas = new CanvasWidget(this);
    repl = new REPLWidget(this);



    layout->addWidget(message);
    layout->addWidget(canvas);
    layout->addWidget(repl);


    layout->setStretchFactor(message, 1);
    layout->setStretchFactor(canvas, 10);
    layout->setStretchFactor(repl, 1);
    setLayout(layout);



    QObject::connect(repl, &REPLWidget::lineEntered, &interp, &QtInterpreter::parseAndEvaluate);
    QObject::connect(&interp, &QtInterpreter::info, message, &MessageWidget::info);
    QObject::connect(&interp, &QtInterpreter::drawGraphic, canvas, &CanvasWidget::addGraphic);
    QObject::connect(&interp, &QtInterpreter::error, message, &MessageWidget::error);
}

/* Event used to read inputted file and display when canvas is ready */
void MainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    if (!file.empty())
    {
        QFile qFile(file.c_str());
        

        if (qFile.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&qFile);
            interp.parseAndEvaluate(stream.readAll());
            qFile.close();
        }
        else
        {
            message->error(QString::fromStdString("Error openning " + file));

        }
    }
}