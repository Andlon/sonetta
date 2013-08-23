#include "uistatecoordinator.h"

#include <QVariantMap>
#include <QDebug>

namespace Sonetta {

UiStateCoordinator::UiStateCoordinator(QObject *parent) :
    QObject(parent), m_currentStateIndex(-1)
{
    m_fonts = new QQmlPropertyMap(this);
    m_colors = new QQmlPropertyMap(this);
    m_misc = new QQmlPropertyMap(this);
}

QVariant UiStateCoordinator::state() const
{
    if (m_states.isEmpty())
        return QVariantMap();

    return m_states.at(m_currentStateIndex);
}

void UiStateCoordinator::pushState(const QVariant &state)
{
    ++m_currentStateIndex;
    int removeCount = m_states.count() - m_currentStateIndex;
    m_states.remove(m_currentStateIndex, removeCount);
    m_states.append(state);

    emit stateChanged(Forward);
}

void UiStateCoordinator::initState(const QVariant &state)
{
    if (m_states.isEmpty())
    {
        m_currentStateIndex = 0;
        m_states.append(state);
        emit stateChanged(None);
    }
}

void UiStateCoordinator::resetState(const QVariant &state, Direction direction)
{
    m_currentStateIndex = 0;
    m_states.clear();
    m_states.append(state);

    emit stateChanged(direction);
}

bool UiStateCoordinator::hasPreviousState() const
{
    return m_currentStateIndex > 0;
}

bool UiStateCoordinator::hasNextState() const
{
    return m_currentStateIndex < m_states.count() - 1;
}

void UiStateCoordinator::nextState()
{
    if (hasNextState())
    {
        ++m_currentStateIndex;
        emit stateChanged(Forward);
    }
}

void UiStateCoordinator::previousState()
{
    if (hasPreviousState())
    {
        --m_currentStateIndex;
        emit stateChanged(Backward);
    }
}

void UiStateCoordinator::setColorProperties(const QVariant &colors)
{
    if (colors.type() == QVariant::Map)
    {
        const QVariantMap colorMap = colors.toMap();

        m_colors->deleteLater();
        m_colors = new QQmlPropertyMap(this);

        // Insert key-value pairs to property map
        QVariantMap::ConstIterator i = colorMap.constBegin();
        while (i != colorMap.constEnd())
        {
            m_colors->insert(i.key(), i.value());
            ++i;
        }

        emit colorsChanged();
    }
}

void UiStateCoordinator::setFontProperties(const QVariant &fonts)
{
    if (fonts.type() == QVariant::Map)
    {
        const QVariantMap fontMap = fonts.toMap();

        m_fonts->deleteLater();
        m_fonts = new QQmlPropertyMap(this);

        // Insert key-value pairs to property map
        QVariantMap::ConstIterator i = fontMap.constBegin();
        while (i != fontMap.constEnd())
        {
            m_fonts->insert(i.key(), i.value());
            ++i;
        }

        emit fontsChanged();
    }
}

void UiStateCoordinator::setMiscProperties(const QVariant &misc)
{
    if (misc.type() == QVariant::Map)
    {
        const QVariantMap miscMap = misc.toMap();

        m_misc->deleteLater();
        m_misc = new QQmlPropertyMap(this);

        // Insert key-value pairs to property map
        QVariantMap::ConstIterator i = miscMap.constBegin();
        while (i != miscMap.constEnd())
        {
            m_misc->insert(i.key(), i.value());
            ++i;
        }

        emit miscChanged();
    }
}

QObject * UiStateCoordinator::colors() const
{
    return m_colors;
}

QObject * UiStateCoordinator::fonts() const
{
    return m_fonts;
}

QObject * UiStateCoordinator::misc() const
{
    return m_misc;
}

}
