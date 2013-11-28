#include "settings.h"

#include <QDebug>

namespace Sonetta {

Settings::Settings(QObject *parent)
    :   QObject(parent), m_mouseEnabled(false)
{
    syncSettings();
}

bool Settings::mouseEnabled() const
{
    return m_mouseEnabled;
}

void Settings::setMouseEnabled(bool enabled)
{
    m_mouseEnabled = enabled;
    emit mouseEnabledChanged();
}

QStringList Settings::loadSearchHistory()
{
    QVariant history = m_settings.value("search/history", QStringList());
    if (static_cast<QMetaType::Type>(history.type()) != QMetaType::QStringList)
        qWarning() << "Search history is not a valid string list.";

    return history.toStringList();
}

void Settings::commitSearchHistory(const QStringList &history)
{
    m_settings.setValue("search/history", history);
}

void Settings::syncSettings()
{
    if (m_settings.contains("input/enablemouse"))
        m_mouseEnabled = m_settings.value("input/enablemouse", m_mouseEnabled).toBool();
}




}
