#ifndef LIRCREMOTE_H
#define LIRCREMOTE_H

#include <QObject>
#include <QLocalSocket>

class LircRemote : public QObject
{
    Q_OBJECT
public:
    explicit LircRemote(QObject *parent = 0);
    
    void attach();

private slots:
    void readData();
    void handleError(QLocalSocket::LocalSocketError);

private:
    QLocalSocket * m_socket;
    
};

#endif // LIRCREMOTE_H
