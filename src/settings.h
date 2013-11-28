#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QStringList>
#include <QSettings>

namespace Sonetta {

class Settings : public QObject {
    Q_OBJECT
public:
    explicit Settings(QObject * parent = 0);

    QStringList loadSearchHistory();
    void commitSearchHistory(const QStringList & history);

signals:


private:
    QSettings m_settings;
};

}
