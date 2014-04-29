#include <QString>
#include <QtTest>
#include "../../src/quick/quickglobalstatemachine.h"

using Sonetta::QuickGlobalStateMachine;
using Sonetta::QuickGlobalStateTransition;

class QuickGlobalStateMachineTest : public QObject
{
    Q_OBJECT

public:
    QuickGlobalStateMachineTest() { }

private Q_SLOTS:
    void init() {
        machine = new QuickGlobalStateMachine;
        machine->initialize("initial");
        transition = new QuickGlobalStateTransition;
        enterSpy = new QSignalSpy(machine, SIGNAL(enter(QString,QVariant)));
        leaveSpy = new QSignalSpy(machine, SIGNAL(leave(QString,QVariant)));
    }

    void cleanup() {
        delete leaveSpy;
        delete enterSpy;
        delete transition;
        delete machine;
    }

    void testSimpleStateChange() {
        machine->push("next");
        QCOMPARE(leaveSpy->count(), 1);
        QCOMPARE(enterSpy->count(), 1);

        QList<QVariant> arg = enterSpy->takeFirst();
        QCOMPARE(arg[0].toString(), QStringLiteral("next"));
        QCOMPARE(arg[1], QVariant());
    }

    void testRepeatedSimpleStateChange() {
        machine->push("next");
        machine->push("next2");
        QCOMPARE(leaveSpy->count(), 2);
        QCOMPARE(enterSpy->count(), 2);

        QList<QVariant> arg1 = enterSpy->takeFirst();
        QList<QVariant> arg2 = enterSpy->takeFirst();
        QCOMPARE(arg1[0].toString(), QStringLiteral("next"));
        QCOMPARE(arg1[1], QVariant());
        QCOMPARE(arg2[0].toString(), QStringLiteral("next2"));
        QCOMPARE(arg2[1], QVariant());
    }

    void testSingleTransition() {
        QSignalSpy initializationSpy(transition, SIGNAL(initialized(QString,QVariant,QString,QVariant)));
        QSignalSpy finalizationSpy(transition, SIGNAL(finalized(QuickGlobalStateTransition*)));

        machine->registerTransition("initial", "next", transition);
        machine->push("next");

        QCOMPARE(initializationSpy.count(), 1);
        QList<QVariant> arg = initializationSpy.takeFirst();
        QCOMPARE(arg[0].toString(), QStringLiteral("initial"));
        QCOMPARE(arg[2].toString(), QStringLiteral("next"));

        transition->finalize();
        QCOMPARE(finalizationSpy.count(), 1);
        QCOMPARE(enterSpy->count(), 1);
        QCOMPARE(leaveSpy->count(), 1);
    }

private:
    QuickGlobalStateMachine * machine;
    QuickGlobalStateTransition * transition;
    QSignalSpy * enterSpy;
    QSignalSpy * leaveSpy;
};

QTEST_APPLESS_MAIN(QuickGlobalStateMachineTest)

#include "quickglobalstatemachinetest.moc"
