#include "settings.h"

#include <QDebug>

namespace {

const QString LIRCDELAY_SETTING =      QStringLiteral("input/lircdelay");
const QString ENABLEMOUSE_SETTING =    QStringLiteral("input/enablemouse");
const QString SEARCHISTORY_SETTING =   QStringLiteral("search/history");

}

namespace Sonetta {

Settings::Settings(QObject *parent)
    :   QObject(parent), m_mouseEnabled(false), m_lircDelay(3)
{
    syncSettings();
}

bool Settings::mouseEnabled() const
{
    return m_mouseEnabled;
}

void Settings::setMouseEnabled(bool enabled)
{
    if (enabled != m_mouseEnabled)
    {
        m_mouseEnabled = enabled;
        commitSetting(ENABLEMOUSE_SETTING, m_mouseEnabled);
        emit mouseEnabledChanged();
    }
}

int Settings::lircDelay() const
{
    return m_lircDelay;
}

void Settings::setLircDelay(int delay)
{
    if (delay >= 0 && delay != m_lircDelay)
    {
        m_lircDelay = delay;
        commitSetting(LIRCDELAY_SETTING, m_lircDelay);
        emit lircDelayChanged();
    }
}

QStringList Settings::loadSearchHistory()
{
    QVariant history = m_settings.value(SEARCHISTORY_SETTING, QStringList());
    if (static_cast<QMetaType::Type>(history.type()) != QMetaType::QStringList)
        qWarning() << "Search history is not a valid string list.";

    return history.toStringList();
}

void Settings::commitSearchHistory(const QStringList &history)
{
    m_settings.setValue("search/history", history);
}

void Settings::commitSetting(const QString &setting, const QVariant &value)
{
    m_settings.setValue(setting, value);
}

void Settings::syncSettings()
{
    m_mouseEnabled = m_settings.value(ENABLEMOUSE_SETTING, m_mouseEnabled).toBool();
    m_lircDelay = m_settings.value(LIRCDELAY_SETTING, m_lircDelay).toInt();
}




}
