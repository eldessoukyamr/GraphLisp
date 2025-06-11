#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <string>

#include <QWidget>

#include "qt_interpreter.hpp"


class QVBoxLayout;
class MessageWidget;
class CanvasWidget;
class REPLWidget;

class MainWindow : public QWidget {
Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    MainWindow(std::string filename, QWidget *parent = nullptr);
protected:
    void showEvent(QShowEvent* event) override;
private:
    std::string file;
    QtInterpreter interp;


    QVBoxLayout* layout;

    MessageWidget* message;
    CanvasWidget* canvas;
    REPLWidget* repl;
};


#endif

