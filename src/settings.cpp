#include "settings.h"

#include <QDebug>

namespace Sonetta {

Settings::Settings(QObject *parent)
    :   QObject(parent)
{

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




}
