#pragma once

#include <QObject>
#include <QLocalSocket>
#include <QXmlStreamReader>
#include <QHash>
#include <QString>

#include "navigation.h"
#include "../utilities/pointers.h"

class Lircmap
{
public:
    Navigation::Button lookup(const QString &remote, const QString &lircButton) const;

    bool parse(const QString &filepath);
    void clear();

private:
    void readLircmapElement();
    void readRemoteElement();

    typedef QHash<QString, Navigation::Button> RemoteMapping;

    QHash<QString, RemoteMapping> m_remotes;
    QXmlStreamReader m_reader;
};

class LircClient : public QObject
{
    Q_OBJECT
public:
    explicit LircClient(QObject *parent = 0);
    
    void attach();

    int delay() const;
    void setDelay(int delay);

private slots:
    void readData();
    void handleError(QLocalSocket::LocalSocketError);

private:
    void populateLircmap();

    int                                 m_delay;
    Lircmap                             m_map;
    ObjectScopedPointer<QLocalSocket>   m_socket;
};
