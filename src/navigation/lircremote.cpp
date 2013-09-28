#include "lircremote.h"
#include "navigation.h"

#include <QDebug>

static const char LIRC_ADDRESS[] = "/var/run/lirc/lircd";

LircRemote::LircRemote(QObject *parent) :
    QObject(parent), m_socket(new QLocalSocket(this))
{
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
                              this, SLOT(handleError(QLocalSocket::LocalSocketError)));
}

void LircRemote::attach()
{
    m_socket->connectToServer(QString(LIRC_ADDRESS), QIODevice::ReadOnly);
}

void LircRemote::readData()
{
    while (m_socket->bytesAvailable() > 0)
    {
        QString line = QString(m_socket->readLine()).simplified();

        QStringList parts = line.split(" ", QString::SkipEmptyParts);
        if (parts.count() == 4)
        {
            bool ok; // Holds whether the conversion to integer was successful
            int repeatCount = parts[1].toInt(&ok, 16);

            if (ok && (repeatCount == 0 || repeatCount >= 3))
            {
                QString button = parts[2];
                QString remote = parts[3];

                //                    if (remote == "mceusb")
                //                    {
                Navigation::Key key = Navigation::Undefined;

                if (button == "KEY_BACK")
                    key = Navigation::Back;
                else if (button == "KEY_OK")
                    key = Navigation::OK;
                else if (button == "KEY_RIGHT")
                    key = Navigation::Right;
                else if (button == "KEY_LEFT")
                    key = Navigation::Left;
                else if (button == "KEY_UP")
                    key = Navigation::Up;
                else if (button == "KEY_DOWN")
                    key = Navigation::Down;
                else if (button == "KEY_PLAY")
                    key = Navigation::Play;
                else if (button == "KEY_PLAYPAUSE")
                    key = Navigation::PlayPause;
                else if (button == "KEY_PAUSE")
                    key = Navigation::Pause;
                else if (button == "KEY_RECORD")
                    key = Navigation::Record;
                else if (button == "KEY_AGAIN")
                    key = Navigation::Previous;
                else if (button == "KEY_NEXT")
                    key = Navigation::Next;
                else if (button == "KEY_STOP")
                    key = Navigation::Stop;
                else if (button == "Forward")
                    key = Navigation::FastForward;
                else if (button == "Rewind")
                    key = Navigation::Rewind;
                else if (button == "ChanDown")
                    key = Navigation::ChannelDown;
                else if (button == "ChanUp")
                    key = Navigation::ChannelUp;
                else if (button == "VolDown")
                    key = Navigation::VolumeDown;
                else if (button == "VolUp")
                    key = Navigation::VolumeUp;
                else if (button == "Mute")
                    key = Navigation::Mute;

                if (key != Navigation::Undefined)
                {
                    NavEvent navEvent(key, repeatCount > 0);
                    Navigation::dispatchNavigationEvent(&navEvent);
                }
                //                    }


            }
        }
        else
        {
            qDebug() << ("Unhandled data received from LIRC. Raw message: " + line);
        }
    }
}

void LircRemote::handleError(QLocalSocket::LocalSocketError err)
{
    if (err == QLocalSocket::ConnectionRefusedError)
    {
        qWarning() << "LIRC connection refused. ";
    }
    else if (err == QLocalSocket::ServerNotFoundError)
    {
        qWarning() << "LIRC server not found. ";
    }
    else if (err == QLocalSocket::PeerClosedError)
    {
        qWarning() << "Connection closed by peer. ";
    }
    else if (err == QLocalSocket::SocketAccessError)
    {
        qWarning() << "Socket access error. ";
    }
    else
    {
        qWarning() << "Unknown socket error occured";
    }
}
