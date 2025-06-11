#include "message_widget.hpp"

#include <QLabel>
#include <QLayout>
#include <iostream>
#include <QLineEdit>
#include <QHBoxLayout>

/* Constructor for message widget */
MessageWidget::MessageWidget(QWidget *parent) : QWidget(parent) {
    QHBoxLayout* layout = new QHBoxLayout(this);

    messageName = new QLabel(this);
    messageField = new QLineEdit(this);


    messageName->setText("message:");



    messageField->setEnabled(true);
    messageField->setReadOnly(true);
    messageField->setText("");


    layout->addWidget(messageName, 1);
    layout->addWidget(messageField, 7);


    setLayout(layout);
}

/* Slot that receives a message to display in the message widget */
void MessageWidget::info(QString message) {
    // TODO: your code here...
    messageField->setStyleSheet("QLineEdit {color: black}");
    messageField->setText(message);
    messageField->deselect();
}

/* Slot that receives a error message to display in the message widget */
void MessageWidget::error(QString message) {
    // TODO: your code here...
    messageField->setStyleSheet("QLineEdit {color: black; selection-background-color: red}");
    messageField->setText(QString("Error: ") + message);
    messageField->selectAll();
}

void MessageWidget::clear() {
    // TODO: your code here...
    messageField->clear();
}

