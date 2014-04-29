#pragma once

#include <QObject>
#include <QVariant>
#include <QMultiHash>
#include <QStack>
#include <QPointer>

namespace Sonetta {

class QuickGlobalStateTransition : public QObject {
Q_OBJECT
public:
    explicit QuickGlobalStateTransition(QObject * parent = 0) : QObject(parent) { }

    void initialize(const QString & state, const QVariant & parameters) { emit onInitialized(state, parameters); }
    void finalize() { emit onFinalized(this); }

signals:
    void onInitialized(const QString & state, const QVariant & parameters);
    void onFinalized(QuickGlobalStateTransition * transition);
};

class QuickGlobalStateMachine : public QObject {
Q_OBJECT
public:
    explicit QuickGlobalStateMachine(QObject * parent = 0) : QObject(parent) { }

    Q_INVOKABLE void registerTransition(const QString & from, const QString & to, QuickGlobalStateTransition * transition);
    //Q_INVOKABLE void deregisterTransition(QuickGlobalStateTransition * transition);

    Q_INVOKABLE void push(const QString & state, const QVariant & parameters);
    Q_INVOKABLE void pop();

signals:
    void onLeave(const QString & state, const QVariant & parameters);
    void onEnter(const QString & state, const QVariant & parameters);

private slots:
    void handleFinalization(QuickGlobalStateTransition * transition);

private:
    typedef QHash< QString, QMultiHash<QString, QPointer<QuickGlobalStateTransition> > > TransitionRegistry;
    struct State {
        State() { }
        State(QString name, QVariant parameters) : name(name), parameters(parameters) { }
        QString name;
        QVariant parameters;
    };

    void transition(const State & previous, const State & next);

    TransitionRegistry  m_transitions;
    QStack<State>       m_states;

    QVector< QPointer<QuickGlobalStateTransition> > m_ongoingTransitions;
};

}
