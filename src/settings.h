#pragma once

#include <QObject>
#include <QScopedPointer>
#include <QStringList>
#include <QSettings>

namespace Sonetta {

class Settings : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool mouseEnabled READ mouseEnabled WRITE setMouseEnabled NOTIFY mouseEnabledChanged)
public:
    explicit Settings(QObject * parent = 0);

    bool mouseEnabled() const;
    void setMouseEnabled(bool enabled);

    QStringList loadSearchHistory();
    void commitSearchHistory(const QStringList & history);

signals:
    void mouseEnabledChanged();

private:
    void syncSettings();

    QSettings m_settings;

    bool m_mouseEnabled;
};

}
