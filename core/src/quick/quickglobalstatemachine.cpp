#include "quickglobalstatemachine.h"

#include <QtDebug>

void QuickGlobalStateTransition::initialize(const QString & from, const QVariant & fromParameters,
                                            const QString & to, const QVariant & toParameters)
{
    Q_ASSERT(m_finalized == true);
    m_finalized = false;
    emit finalizedChanged();
    emit initialized(from, fromParameters, to, toParameters);
}

void QuickGlobalStateTransition::finalize() {
    if (m_finalized)
    {
        qCritical() << "GlobalStateTransition: Attempting to finalized an already finalized transition.";
        return;
    }

    m_finalized = true;
    emit finalizedChanged();
    emit finalized(this);
}

void QuickGlobalStateMachine::registerTransition(const QString &from, const QString &to, QuickGlobalStateTransition *transition)
{
    Q_ASSERT(transition != nullptr);
    connect(transition, &QuickGlobalStateTransition::finalized, this, &QuickGlobalStateMachine::handleFinalization, Qt::UniqueConnection);
    m_transitions[from].insert(to, QPointer<QuickGlobalStateTransition>(transition));
}

void QuickGlobalStateMachine::initialize(const QString &state, const QVariant &parameters)
{
    if (m_states.isEmpty())
    {
        m_states.push(State(state, parameters));
        emit enter(state, parameters);
    }
    else
    {
        qWarning() << "GlobalStateMachine: Can not initialize state when a state already exists.";
    }
}

void QuickGlobalStateMachine::push(const QString &state, const QVariant &parameters)
{
    if (m_leaving)
    {
        qCritical() << "GlobalStateMachine: Can not push state when leaving state.";
        return;
    }

    if (!m_ongoingTransitions.isEmpty())
    {
        qCritical() << "GlobalStateMachine: Can not push state during transition.";
        return;
    }

    if (!m_states.isEmpty())
    {
        const State currentState = m_states.top();
        const State newState (state, parameters);
        m_states.push(newState);
        m_leaving = true;
        emit leave(currentState.name, currentState.parameters);
        m_leaving = false;
        transition(currentState, newState);
    }
}

void QuickGlobalStateMachine::pop()
{
    if (m_leaving)
    {
        qCritical() << "GlobalStateMachine: Can not pop state when leaving state.";
        return;
    }

    if (!m_ongoingTransitions.isEmpty())
    {
        qCritical() << "GlobalStateMachine: Can not pop state during transition.";
        return;
    }

    // Silently forbid popping when there is no previous state
    if (m_states.count() > 1)
    {
        const State currentState = m_states.pop();
        m_leaving = true;
        emit leave(currentState.name, currentState.parameters);
        m_leaving = false;
        const State & newState = m_states.top();
        transition(currentState, newState);
    }
}

void QuickGlobalStateMachine::handleFinalization(QuickGlobalStateTransition *transition)
{
    int index = m_ongoingTransitions.indexOf(transition);
    Q_ASSERT_X(index > -1, "QuickGlobalStateMachine::handleFinalization", "Finalized transition must be an ongoing transition.");

    m_ongoingTransitions.remove(index);
    finalizeState();
}

void QuickGlobalStateMachine::transition(const QuickGlobalStateMachine::State &previous, const QuickGlobalStateMachine::State &next)
{
    auto transitions = m_transitions.value(previous.name).values(next.name);

    // Add wildcard transitions
    transitions.append(m_transitions.value("*").values(next.name));
    transitions.append(m_transitions.value(previous.name).values("*"));
    transitions.append(m_transitions.value("*").values("*"));

    QVector<QuickGlobalStateTransition *> validTransitions;

    // We need to make a list of all ongoing transitions before we initialize them,
    // in case some transitions finalize within the same block of execution as the initialization
    for (auto & transition : transitions)
    {
        if (transition.isNull()){
            // Clean up or do something clever here. For now, just warn.
            qWarning() << "GlobalStateMachine: Null transition. Please fix.";
        }
        else {
            m_ongoingTransitions.append(transition);
            validTransitions.append(transition.data());
        }
    }

    for (auto & transition : validTransitions)
        transition->initialize(previous.name, previous.parameters, next.name, next.parameters);

    // Finalize immediately if we have no valid transitions
    if (validTransitions.isEmpty())
        finalizeState();
}

void QuickGlobalStateMachine::finalizeState()
{
    if (m_ongoingTransitions.isEmpty())
    {
        Q_ASSERT(!m_states.isEmpty());
        const State & state = m_states.top();
        emit enter(state.name, state.parameters);
    }
}
