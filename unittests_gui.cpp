// TODO: ADD YOUR Qt UNIT TESTS HERE
#include <QDebug>
#include <QtTest/QtTest>
#include <QtWidgets>
#include <iostream>
#include <qt_interpreter.hpp>
#include <main_window.hpp>
#include <qgraphics_arc_item.hpp>
#include <repl_widget.hpp>
#include <message_widget.hpp>
#include <canvas_widget.hpp>


class IntegratedQtGuiTests : public QObject {
    Q_OBJECT

private slots:
    // QtInterpreter
    void parseAndEvaluate_invalidParse_emitsError();
    void parseAndEvaluate_evalError_emitsError();
    void parseAndEvaluate_draw_success();

    // Widgets
    void testCanvasWidget_addGraphic();
    void testMessageWidget_infoAndError();
    void testMessageWidget_clear();
    void testREPLWidget_emitsSignal();
    void testQGraphicsArcItem_paint();
    void testMainWindow_fileLoad();
};

class QtInterpreterTestWrapper : public QtInterpreter {
    Q_OBJECT
public:
    QString lastError;
    QString lastInfo;
    int drawCount = 0;

    QtInterpreterTestWrapper() {
        connect(this, &QtInterpreter::error, this, &QtInterpreterTestWrapper::handleError);
        connect(this, &QtInterpreter::info, this, &QtInterpreterTestWrapper::handleInfo);
        connect(this, &QtInterpreter::drawGraphic, this, &QtInterpreterTestWrapper::handleDraw);
    }

public slots:
    void handleError(const QString &msg) { lastError = msg; }
    void handleInfo(const QString &msg) { lastInfo = msg; }
    void handleDraw(QGraphicsItem*) { drawCount++; }
};

// QtInterpreter
void IntegratedQtGuiTests::parseAndEvaluate_invalidParse_emitsError() {
    QtInterpreterTestWrapper interp;
    interp.parseAndEvaluate("(+ 1 2");
    QCOMPARE(interp.lastError, QString("Unable to parse"));
}

void IntegratedQtGuiTests::parseAndEvaluate_evalError_emitsError() {
    QtInterpreterTestWrapper interp;
    interp.parseAndEvaluate("(/ 1 0)");
    QCOMPARE(interp.lastError, QString("Test"));
}

void IntegratedQtGuiTests::parseAndEvaluate_draw_success() {
    QtInterpreterTestWrapper interp;
    interp.parseAndEvaluate("(draw (point 2 4))");
    QCOMPARE(interp.lastInfo, QString("(2,4)"));
    QCOMPARE(interp.drawCount, 1);
}

// Widget tests
void IntegratedQtGuiTests::testCanvasWidget_addGraphic() {
    CanvasWidget canvas;
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(0, 0, 10, 10);
    canvas.addGraphic(item);
    QVERIFY(true);
}

void IntegratedQtGuiTests::testMessageWidget_infoAndError() {
    MessageWidget widget;
    widget.info("this is info");
    QCOMPARE(widget.findChild<QLineEdit *>()->text(), QString("this is info"));

    widget.error("this is an error");
    QCOMPARE(widget.findChild<QLineEdit *>()->text(), QString("Error: this is an error"));
}

void IntegratedQtGuiTests::testMessageWidget_clear() {
    MessageWidget widget;
    widget.info("to be cleared");
    widget.clear();
    QCOMPARE(widget.findChild<QLineEdit *>()->text(), QString(""));
}

void IntegratedQtGuiTests::testREPLWidget_emitsSignal() {
    REPLWidget widget;
    QSignalSpy spy(&widget, SIGNAL(lineEntered(QString)));
    QLineEdit *entry = widget.findChild<QLineEdit *>();
    entry->setText("(+ 1 2)");
    QTest::keyClick(entry, Qt::Key_Return);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QString("(+ 1 2)"));
}

void IntegratedQtGuiTests::testQGraphicsArcItem_paint() {
    QGraphicsArcItem arc(0, 0, 100, 100);
    arc.setStartAngle(30 * 16);
    arc.setSpanAngle(60 * 16);
    QPixmap pixmap(120, 120);
    pixmap.fill(Qt::white);
    QPainter painter(&pixmap);
    QStyleOptionGraphicsItem option;
    arc.paint(&painter, &option, nullptr);
    QVERIFY(true);
}

void IntegratedQtGuiTests::testMainWindow_fileLoad() {
    MainWindow window("nonexistent_file.slp");
    window.show();
    QVERIFY(window.isVisible());
}

QTEST_MAIN(IntegratedQtGuiTests)

#include "unittests_gui.moc"
