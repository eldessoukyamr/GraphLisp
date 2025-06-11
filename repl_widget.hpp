#ifndef REPL_WIDGET_HPP
#define REPL_WIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QVector>

class REPLWidget : public QWidget {
Q_OBJECT
    QLabel* userTitle;
    QLineEdit* userEntry;
public:

    REPLWidget(QWidget *parent = nullptr);

signals:

    void lineEntered(QString entry);

private slots:
    void changed();
};

#endif
