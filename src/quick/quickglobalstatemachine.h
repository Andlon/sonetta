#pragma once

#include <QObject>
#include <QVariant>
#include <QMultiHash>
#include <QStack>
#include <QPointer>

namespace Sonetta {

class QuickGlobalStateTransition : public QObject {
Q_OBJECT

    Q_PROPERTY(bool finalized READ isFinalized NOTIFY finalizedChanged)
public:
    explicit QuickGlobalStateTransition(QObject * parent = 0) : QObject(parent), m_finalized(true) { }

    void initialize(const QString & state, const QVariant & parameters);
    Q_INVOKABLE void finalize();

    bool isFinalized() const { return m_finalized; }

signals:
    void finalizedChanged();

    void initialized(const QString & state, const QVariant & parameters);
    void finalized(QuickGlobalStateTransition * transition);

private:
    bool m_finalized;
};

class QuickGlobalStateMachine : public QObject {
Q_OBJECT
public:
    explicit QuickGlobalStateMachine(QObject * parent = 0) : QObject(parent), m_leaving(false) { }

    Q_INVOKABLE void registerTransition(const QString & from, const QString & to, QuickGlobalStateTransition * transition);
    //Q_INVOKABLE void deregisterTransition(QuickGlobalStateTransition * transition);

    Q_INVOKABLE void initialize(const QString & state, const QVariant & parameters);
    Q_INVOKABLE void push(const QString & state, const QVariant & parameters);
    Q_INVOKABLE void pop();

signals:
    void leave(const QString & state, const QVariant & parameters);
    void enter(const QString & state, const QVariant & parameters);

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
    void finalizeState();

    TransitionRegistry  m_transitions;
    QStack<State>       m_states;
    bool                m_leaving;

    QVector< QPointer<QuickGlobalStateTransition> > m_ongoingTransitions;
};

}
