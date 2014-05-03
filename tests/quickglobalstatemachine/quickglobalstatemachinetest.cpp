#include <QString>
#include <QtTest>
#include "../../core/src/quick/quickglobalstatemachine.h"

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
        transition2 = new QuickGlobalStateTransition;
        transition3 = new QuickGlobalStateTransition;
        enterSpy = new QSignalSpy(machine, SIGNAL(enter(QString,QVariant)));
        leaveSpy = new QSignalSpy(machine, SIGNAL(leave(QString,QVariant)));
    }

    void cleanup() {
        delete leaveSpy;
        delete enterSpy;
        delete transition3;
        delete transition2;
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
        QSignalSpy init(transition, SIGNAL(initialized(QString,QVariant,QString,QVariant)));
        QSignalSpy final(transition, SIGNAL(finalized(QuickGlobalStateTransition*)));

        machine->registerTransition("initial", "next", transition);
        machine->push("next");

        QCOMPARE(init.count(), 1);
        QList<QVariant> arg = init.takeFirst();
        QCOMPARE(arg[0].toString(), QStringLiteral("initial"));
        QCOMPARE(arg[2].toString(), QStringLiteral("next"));

        transition->finalize();
        QCOMPARE(final.count(), 1);
        QCOMPARE(enterSpy->count(), 1);
        QCOMPARE(leaveSpy->count(), 1);
    }

    void testRepeatedSingleTransition() {
        QSignalSpy init(transition, SIGNAL(initialized(QString,QVariant,QString,QVariant)));
        QSignalSpy final(transition, SIGNAL(finalized(QuickGlobalStateTransition*)));

        machine->registerTransition("initial", "next", transition);
        machine->registerTransition("next", "initial", transition);
        machine->push("next");
        transition->finalize();
        machine->push("initial");
        transition->finalize();
        machine->push("next");
        transition->finalize();

        QCOMPARE(init.count(), 3);
        QCOMPARE(final.count(), 3);

        // Extra test that the transition does not trigger on an unregistered state change
        machine->push("next");

        QCOMPARE(init.count(), 3);
        QCOMPARE(final.count(), 3);
    }

    void testMultipleTransitions() {
        QSignalSpy init1(transition, SIGNAL(initialized(QString,QVariant,QString,QVariant)));
        QSignalSpy init2(transition2, SIGNAL(initialized(QString,QVariant,QString,QVariant)));
        QSignalSpy final1(transition, SIGNAL(finalized(QuickGlobalStateTransition*)));
        QSignalSpy final2(transition2, SIGNAL(finalized(QuickGlobalStateTransition*)));

        machine->registerTransition("initial", "next", transition);
        machine->registerTransition("initial", "next", transition2);
        machine->push("next");
        QCOMPARE(init1.count(), 1);
        QCOMPARE(init2.count(), 1);
        transition->finalize();
        transition2->finalize();
        QCOMPARE(final1.count(), 1);
        QCOMPARE(final2.count(), 1);
    }

    void testRepeatedMultipleTransitions() {
        QSignalSpy init1(transition, SIGNAL(initialized(QString,QVariant,QString,QVariant)));
        QSignalSpy init2(transition2, SIGNAL(initialized(QString,QVariant,QString,QVariant)));
        QSignalSpy final1(transition, SIGNAL(finalized(QuickGlobalStateTransition*)));
        QSignalSpy final2(transition2, SIGNAL(finalized(QuickGlobalStateTransition*)));

        machine->registerTransition("initial", "next", transition);
        machine->registerTransition("initial", "next", transition2);
        machine->registerTransition("next", "initial", transition);
        machine->registerTransition("next", "initial", transition2);

        machine->push("next");
        transition->finalize();
        transition2->finalize();
        machine->push("initial");
        transition->finalize();
        transition2->finalize();
        machine->push("next");
        transition->finalize();
        transition2->finalize();

        QCOMPARE(init1.count(), 3);
        QCOMPARE(init2.count(), 3);
        QCOMPARE(final1.count(), 3);
        QCOMPARE(final2.count(), 3);

        // Test that no transition is used for unregistered state change (next -> next)
        machine->push("next");
        QCOMPARE(init1.count(), 3);
        QCOMPARE(init2.count(), 3);
        QCOMPARE(final1.count(), 3);
        QCOMPARE(final2.count(), 3);
    }

    void testWildcardTransition() {
        QSignalSpy final1(transition, SIGNAL(finalized(QuickGlobalStateTransition*)));
        QSignalSpy final2(transition2, SIGNAL(finalized(QuickGlobalStateTransition*)));
        QSignalSpy final3(transition3, SIGNAL(finalized(QuickGlobalStateTransition*)));

        machine->registerTransition("*", "next", transition);
        machine->registerTransition("initial", "*", transition2);
        machine->registerTransition("*", "*", transition3);
        machine->push("next");
        transition->finalize();
        transition2->finalize();
        transition3->finalize();
        QCOMPARE(final1.count(), 1);
        QCOMPARE(final2.count(), 1);
        QCOMPARE(final3.count(), 1);

        // Additionally check that only the double wildcard triggers
        // on the next transition
        machine->push("arbitrary");
        transition3->finalize();
        QCOMPARE(final1.count(), 1);
        QCOMPARE(final2.count(), 1);
        QCOMPARE(final3.count(), 2);
    }

private:
    QuickGlobalStateMachine * machine;
    QuickGlobalStateTransition * transition;
    QuickGlobalStateTransition * transition2;
    QuickGlobalStateTransition * transition3;
    QSignalSpy * enterSpy;
    QSignalSpy * leaveSpy;
};

QTEST_APPLESS_MAIN(QuickGlobalStateMachineTest)

#include "quickglobalstatemachinetest.moc"
