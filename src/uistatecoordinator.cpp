#include "uistatecoordinator.h"

#include <QVariantMap>
#include <QDebug>

namespace Sonetta {

UIStateCoordinator::UIStateCoordinator(QObject *parent) :
    QObject(parent)
{
    m_fonts = new QQmlPropertyMap(this);
    m_colors = new QQmlPropertyMap(this);
    m_misc = new QQmlPropertyMap(this);
}

QVariant UIStateCoordinator::state() const
{
    return m_states.isEmpty() ? QVariantMap() : m_states.top();
}

void UIStateCoordinator::pushState(const QVariant &state)
{
    m_states.push(state);

    emit stateChanged();
}

void UIStateCoordinator::resetState(const QVariant &state)
{
    m_states.clear();
    m_states.push(state);

    emit stateChanged();
}

bool UIStateCoordinator::hasPreviousState() const
{
    return !m_states.empty();
}

void UIStateCoordinator::popState()
{
    if (hasPreviousState())
    {
        m_states.pop();
        emit stateChanged();
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
