#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QStringList>
#include <QSettings>

namespace Sonetta {

class Settings : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool mouseEnabled READ mouseEnabled WRITE setMouseEnabled NOTIFY mouseEnabledChanged)
    Q_PROPERTY(int lircDelay READ lircDelay WRITE setLircDelay NOTIFY lircDelayChanged)
public:
    explicit Settings(QObject * parent = 0);

    bool mouseEnabled() const;
    void setMouseEnabled(bool enabled);

    int lircDelay() const;
    void setLircDelay(int delay);

    QStringList loadSearchHistory();
    void commitSearchHistory(const QStringList & history);

signals:
    void mouseEnabledChanged();
    void lircDelayChanged();

private:
    void commitSetting(const QString & setting, const QVariant & value);
    void syncSettings();

    QSettings m_settings;

    bool    m_mouseEnabled;
    int     m_lircDelay;
};

}
