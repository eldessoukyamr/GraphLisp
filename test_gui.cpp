// DO NOT EDIT THIS FILE.
// you should add your unit tests on unittests_gui.cpp

#include <QDebug>
#include <QtTest/QtTest>
#include <QtWidgets>

#include "canvas_widget.hpp"
#include "main_window.hpp"
#include "message_widget.hpp"
#include "repl_widget.hpp"

#include <iostream>

class TestGUI : public QObject {
Q_OBJECT

public:
private slots:

    void initTestCase();

    void testREPLGood();

    void testREPLBad();

    void testREPLBad2Good();

    void testPoint();

    void testLine();

    void testArc();

    void testRect();

    void testFillRect();

    void testEllipse();

    void testEnvRestore();

    void testMessage();

    void cleanupTestCase();

private:
    MainWindow w;

    REPLWidget *repl;
    QLineEdit *replEdit;
    MessageWidget *message;
    QLineEdit *messageEdit;
    CanvasWidget *canvas;
    QGraphicsScene *scene;
};

void TestGUI::initTestCase() {

    repl = w.findChild<REPLWidget *>();
    QVERIFY2(repl, "Could not find REPLWidget instance in MainWindow instance.");

    replEdit = repl->findChild<QLineEdit *>();
    QVERIFY2(replEdit,
             "Could not find QLineEdit instance in REPLWidget instance.");

    message = w.findChild<MessageWidget *>();
    QVERIFY2(message,
             "Could not find MessageWidget instance in MainWindow instance.");

    messageEdit = message->findChild<QLineEdit *>();
    QVERIFY2(messageEdit,
             "Could not find QLineEdit instance in MessageWidget instance.");

    canvas = w.findChild<CanvasWidget *>();
    QVERIFY2(canvas,
             "Could not find CanvasWidget instance in MainWindow instance.");

    scene = canvas->findChild<QGraphicsScene *>();
    QVERIFY2(scene,
             "Could not find QGraphicsScene instance in CanvasWidget instance.");

    w.setMinimumSize(800, 600);
    w.show();

}

void TestGUI::testREPLGood() {

    QVERIFY(repl && replEdit);
    QVERIFY(message && messageEdit);

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(define a 1)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // check message
    QVERIFY2(messageEdit->isReadOnly(),
             "Expected QLineEdit inside MessageWidget to be read-only.");
    QCOMPARE(messageEdit->text(), QString("(1)"));
}

void TestGUI::testREPLBad() {

    QVERIFY(repl && replEdit);
    QVERIFY(message && messageEdit);

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(foo)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // check message
    QVERIFY2(messageEdit->isReadOnly(),
             "Expected QLineEdit inside MessageWidget to be read-only.");
    QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message.");

    // check background color and selection
    QPalette p = messageEdit->palette();
    QCOMPARE(p.highlight().color(), QColor(Qt::red));
    QVERIFY2(messageEdit->selectedText().startsWith("Error"),
             "Expected error to be selected.");
}

void TestGUI::testREPLBad2Good() {

    QVERIFY(repl && replEdit);
    QVERIFY(message && messageEdit);

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(foo)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // check message
    QVERIFY2(messageEdit->isReadOnly(),
             "Expected QLineEdit inside MessageWidget to be read-only.");
    QVERIFY2(messageEdit->text().startsWith("Error"), "Expected error message.");

    // check background color and selection
    QPalette p = messageEdit->palette();
    QCOMPARE(p.highlight().color(), QColor(Qt::red));
    QVERIFY2(messageEdit->selectedText().startsWith("Error"),
             "Expected error to be selected.");

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(define value 100)");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // check message
    QVERIFY2(messageEdit->isReadOnly(),
             "Expected QLineEdit inside MessageWidget to be read-only.");
    QCOMPARE(messageEdit->text(), QString("(100)"));

    // check background color and selection
    p = messageEdit->palette();
    QVERIFY2(p.highlight().color() != QColor(Qt::red),
             "Did not expect red highlight on successful eval.");
    QVERIFY2(messageEdit->selectedText() == "",
             "Expected no selcted text on successful eval.");
}

void TestGUI::testPoint() {

    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(draw (point 0 0))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // check canvas
    QVERIFY2(scene->itemAt(QPointF(0, 0), QTransform()) != 0,
             "Expected a point in the scene. Not found.");
}

void TestGUI::testLine() {

    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(draw (line (point 10 0) (point 0 10)))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // check canvas
    QVERIFY2(scene->itemAt(QPointF(10, 0), QTransform()) != 0,
             "Expected a line in the scene. Not found.");
    QVERIFY2(scene->itemAt(QPointF(0, 10), QTransform()) != 0,
             "Expected a line in the scene. Not found.");
}

void TestGUI::testArc() {

    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(draw (arc (point 0 0) (point 100 0) pi))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // check canvas
    QVERIFY2(scene->itemAt(QPointF(100, 0), QTransform()) != 0,
             "Expected a point on the arc in the scene. Not found.");
    QVERIFY2(scene->itemAt(QPointF(-100, 0), QTransform()) != 0,
             "Expected a point on the arc in the scene. Not found.");

    QGraphicsItem *temp = scene->itemAt(QPointF(-20, -1), QTransform());
    qDebug() << temp;
    qDebug() << temp->boundingRect();
}

void TestGUI::testRect() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(draw (rect -100 -200 -110 -220))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QGraphicsItem *rect = scene->itemAt(-105, -205, QTransform());
    QVERIFY2(rect->boundingRect() == QRectF(-110.5, -220.5, 11, 21),
             "Expected a rect in the scene. Not found.");

    QVERIFY2(scene->itemAt(-99, -199, QTransform()) == nullptr,
             "Expected no point on the rect in the scene. One found.");
    QVERIFY2(scene->itemAt(-99, -221, QTransform()) == nullptr,
             "Expected no point on the rect in the scene. One found.");
    QVERIFY2(scene->itemAt(-201, -199, QTransform()) == nullptr,
             "Expected no point on the rect in the scene. One found.");
    QVERIFY2(scene->itemAt(-201, -221, QTransform()) == nullptr,
             "Expected no point on the rect in the scene. One found.");
}

void TestGUI::testFillRect() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);
    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(draw (fill_rect (rect 100 100 110 110) 255 255 0))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    QGraphicsItem *rect = scene->itemAt(-105, -205, QTransform());
    QVERIFY2(rect->boundingRect() == QRectF(-110.5, -220.5, 11, 21),
             "Expected a rect in the scene. Not found.");
    QVERIFY2(scene->itemAt(-99, -199, QTransform()) == nullptr,
             "Expected no point on the rect in the scene. One found.");
    QVERIFY2(scene->itemAt(-99, -221, QTransform()) == nullptr,
             "Expected no point on the rect in the scene. One found.");
    QVERIFY2(scene->itemAt(-201, -199, QTransform()) == nullptr,
             "Expected no point on the rect in the scene. One found.");
    QVERIFY2(scene->itemAt(-201, -221, QTransform()) == nullptr,
             "Expected no point on the rect in the scene. One found.");
    qDebug() << QString("Test does not check color. Make sure they are correct.");
}

void TestGUI::testEllipse() {
    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);
    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(draw (ellipse (rect 150 150 160 170)))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);
    QGraphicsItem *rect = scene->itemAt(155, 160, QTransform());
    QVERIFY(rect);
    QVERIFY2(rect->boundingRect() == QRectF(149.5, 149.5, 11, 21),
             "Expected a bounding rect in the scene. Not found.");
    qDebug() << QString("Test does not check the ellipse. Make sure they are correct.");
}

void TestGUI::testEnvRestore() {

    QVERIFY(repl && replEdit);
    QVERIFY(canvas && scene);

    QGraphicsItem *temp2 = scene->itemAt(QPointF(-20, 0), QTransform());
    qDebug() << temp2;

    // send a string to the repl widget
    QTest::keyClicks(replEdit, "(begin (draw (point -20 0)) (define pi 3))");
    QTest::keyClick(replEdit, Qt::Key_Return, Qt::NoModifier);

    // check canvas
    QGraphicsItem *temp = scene->itemAt(QPointF(-20, 0), QTransform());
    qDebug() << temp;

    QVERIFY2(scene->itemAt(QPointF(-20, 0), QTransform()) == 0,
             "Did not expected a point in the scene. One found.");
}

void TestGUI::testMessage() {

    MessageWidget message;

    QLineEdit *messageEdit = message.findChild<QLineEdit *>();
    QVERIFY2(messageEdit,
             "Could not find QLineEdit instance in MessageWidget instance.");

    // check message
    QVERIFY2(messageEdit->isReadOnly(),
             "Expected QLineEdit inside MessageWidget to be read-only.");
    QCOMPARE(messageEdit->text(), QString(""));
}

void TestGUI::cleanupTestCase() {

    QTest::qWait(5000);

}

QTEST_MAIN(TestGUI)

#include "test_gui.moc"
