#include "quickglobalstatemachine.h"


namespace Sonetta {

void QuickGlobalStateMachine::registerTransition(const QString &from, const QString &to, QuickGlobalStateTransition *transition)
{
    Q_ASSERT(transition != nullptr);
    connect(transition, &QuickGlobalStateTransition::onFinalized, this, &QuickGlobalStateMachine::handleFinalization);
    m_transitions[from].insert(to, QPointer<QuickGlobalStateTransition>(transition));
}

void QuickGlobalStateMachine::push(const QString &state, const QVariant &parameters)
{
    if (m_ongoingTransitions.isEmpty() && !m_states.isEmpty())
    {
        const State & currentState = m_states.top();
        const State newState (state, parameters);
        emit onLeave(currentState.name, currentState.parameters);
        m_states.push(newState);
        transition(currentState, newState);
    }
}

void QuickGlobalStateMachine::pop()
{
    if (m_ongoingTransitions.isEmpty() && m_states.count() > 1)
    {
        const State currentState = m_states.pop();
        onLeave(currentState.name, currentState.parameters);
        const State & newState = m_states.top();
        transition(currentState, newState);
    }
}

void QuickGlobalStateMachine::handleFinalization(Sonetta::QuickGlobalStateTransition *transition)
{
    int index = m_ongoingTransitions.indexOf(transition);
    Q_ASSERT_X(index > -1, "QuickGlobalStateMachine::handleFinalization", "Finalized transition must be an ongoing transition.");

    m_ongoingTransitions.remove(index);

    if (m_ongoingTransitions.isEmpty())
    {
        Q_ASSERT(!m_states.isEmpty());
        const State & state = m_states.top();
        emit onEnter(state.name, state.parameters);
    }
}

void QuickGlobalStateMachine::transition(const Sonetta::QuickGlobalStateMachine::State &previous, const Sonetta::QuickGlobalStateMachine::State &next)
{
    const auto transitions =
            m_transitions.value(previous.name, QMultiHash<QString, QPointer<QuickGlobalStateTransition>>())
            .values(next.name);

    for (auto & transition : transitions)
    {
        if (transition.isNull()){

        }
        else {
            // Move this to register
            // connect(transition, &QuickGlobalStateTransition::onFinalized, this, &QuickGlobalStateMachine::handleFinalization);
            m_ongoingTransitions.append(transition);
            transition->initialize(next.name, next.parameters);
        }
    }
}

}
