#include "uistatemanagement.h"
#include <QVariantMap>

#include <QDebug>

UiStateManagement::UiStateManagement(QObject *parent) :
    QObject(parent), m_currentStateIndex(-1)
{
    m_fonts = new QQmlPropertyMap(this);
    m_colors = new QQmlPropertyMap(this);
    m_misc = new QQmlPropertyMap(this);
}

QVariant UiStateManagement::state() const
{
    if (m_states.isEmpty())
        return QVariantMap();

    return m_states.at(m_currentStateIndex);
}

void UiStateManagement::pushState(const QVariant &state)
{
    ++m_currentStateIndex;
    int removeCount = m_states.count() - m_currentStateIndex;
    m_states.remove(m_currentStateIndex, removeCount);
    m_states.append(state);

    emit stateChanged(Forward);
}

void UiStateManagement::initState(const QVariant &state)
{
    if (m_states.isEmpty())
    {
        m_currentStateIndex = 0;
        m_states.append(state);
        emit stateChanged(None);
    }
}

void UiStateManagement::resetState(const QVariant &state, Direction direction)
{
    m_currentStateIndex = 0;
    m_states.clear();
    m_states.append(state);

    emit stateChanged(direction);
}

bool UiStateManagement::hasPreviousState() const
{
    return m_currentStateIndex > 0;
}

bool UiStateManagement::hasNextState() const
{
    return m_currentStateIndex < m_states.count() - 1;
}

void UiStateManagement::nextState()
{
    if (hasNextState())
    {
        ++m_currentStateIndex;
        emit stateChanged(Forward);
    }
}

void UiStateManagement::previousState()
{
    if (hasPreviousState())
    {
        --m_currentStateIndex;
        emit stateChanged(Backward);
    }
}

void UiStateManagement::setColorProperties(const QVariant &colors)
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

void UiStateManagement::setFontProperties(const QVariant &fonts)
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

void UiStateManagement::setMiscProperties(const QVariant &misc)
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

QObject * UiStateManagement::colors() const
{
    return m_colors;
}

QObject * UiStateManagement::fonts() const
{
    return m_fonts;
}

QObject * UiStateManagement::misc() const
{
    return m_misc;
}








