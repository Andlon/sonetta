#include "uistatecoordinator.h"

#include <QVariantMap>
#include <QDebug>

namespace Sonetta {

UIStateCoordinator::UIStateCoordinator(QObject *parent) :
    QObject(parent), m_currentStateIndex(-1)
{
    m_fonts = new QQmlPropertyMap(this);
    m_colors = new QQmlPropertyMap(this);
    m_misc = new QQmlPropertyMap(this);
}

QVariant UIStateCoordinator::state() const
{
    if (m_states.isEmpty())
        return QVariantMap();

    return m_states.at(m_currentStateIndex);
}

void UIStateCoordinator::pushState(const QVariant &state)
{
    ++m_currentStateIndex;
    int removeCount = m_states.count() - m_currentStateIndex;
    m_states.remove(m_currentStateIndex, removeCount);
    m_states.append(state);

    emit stateChanged(Forward);
}

void UIStateCoordinator::initState(const QVariant &state)
{
    if (m_states.isEmpty())
    {
        m_currentStateIndex = 0;
        m_states.append(state);
        emit stateChanged(None);
    }
}

void UIStateCoordinator::resetState(const QVariant &state, Direction direction)
{
    m_currentStateIndex = 0;
    m_states.clear();
    m_states.append(state);

    emit stateChanged(direction);
}

bool UIStateCoordinator::hasPreviousState() const
{
    return m_currentStateIndex > 0;
}

bool UIStateCoordinator::hasNextState() const
{
    return m_currentStateIndex < m_states.count() - 1;
}

void UIStateCoordinator::nextState()
{
    if (hasNextState())
    {
        ++m_currentStateIndex;
        emit stateChanged(Forward);
    }
}

void UIStateCoordinator::previousState()
{
    if (hasPreviousState())
    {
        --m_currentStateIndex;
        emit stateChanged(Backward);
    }
}

void UIStateCoordinator::setColorProperties(const QVariant &colors)
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

void UIStateCoordinator::setFontProperties(const QVariant &fonts)
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

void UIStateCoordinator::setMiscProperties(const QVariant &misc)
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

QObject * UIStateCoordinator::colors() const
{
    return m_colors;
}

QObject * UIStateCoordinator::fonts() const
{
    return m_fonts;
}

QObject * UIStateCoordinator::misc() const
{
    return m_misc;
}

}
