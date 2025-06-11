#include "repl_widget.hpp"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <iostream>

/* Constructor for the input section of MainWindow */
REPLWidget::REPLWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);

    userTitle = new QLabel(this);
    userTitle->setText("slisp>");

    userEntry = new QLineEdit(this);
    userEntry->setText("");

    layout->addWidget(userTitle, 1);
    layout->addWidget(userEntry, 7);




    setLayout(layout);
    QObject::connect(userEntry, &QLineEdit::returnPressed, this, &REPLWidget::changed);

}

/* Event that sends a signal when user hits enter */
void REPLWidget::changed() {
    emit lineEntered(userEntry->text());
    userEntry->clear();
}
