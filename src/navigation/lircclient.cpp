#include "lircclient.h"
#include "navigation.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>

namespace {
const char LIRC_ADDRESS[] = "/var/run/lirc/lircd";

QHash<QString, Navigation::Button> createInternalToNavigationMapping()
{
    QHash<QString, Navigation::Button> mapping;

    mapping.insert("left", Navigation::Left);
    mapping.insert("right", Navigation::Right);
    mapping.insert("up", Navigation::Up);
    mapping.insert("down", Navigation::Down);
    mapping.insert("select", Navigation::OK);
    mapping.insert("back", Navigation::Back);
    mapping.insert("play", Navigation::Play);
    mapping.insert("pause", Navigation::Pause);
    mapping.insert("stop", Navigation::Stop);
    mapping.insert("skipplus", Navigation::Next);
    mapping.insert("skipminus", Navigation::Previous);
    mapping.insert("volumeplus", Navigation::VolumeUp);
    mapping.insert("volumeminus", Navigation::VolumeDown);
    mapping.insert("mute", Navigation::Mute);
    mapping.insert("pageplus", Navigation::ChannelUp);
    mapping.insert("pageminus", Navigation::ChannelDown);
    mapping.insert("record", Navigation::Record);

    return mapping;
}

Navigation::Button translateInternalButton(const QString & internalButton)
{
    static auto mapping = createInternalToNavigationMapping();
    return mapping.value(internalButton.toLower(), Navigation::Undefined);
}

/**
 * @brief getLircmapPaths returns a prioritized list of lircmaps from common locations
 * @return
 */

QStringList getLircmapPaths()
{
    QStringList paths;
    QString home = QDir::homePath();
    QString sonetta = QCoreApplication::applicationDirPath();

    paths << home + "/.config/sonetta/Lircmap.xml";
    paths << home + "/.xbmc/userdata/Lircmap.xml";
    paths << sonetta + "/config/Lircmap.xml";
    paths << "/usr/share/xbmc/userdata/Lircmap.xml";
    paths << "/usr/share/xbmc/system/Lircmap.xml";

    return paths;
}

}

Navigation::Button Lircmap::lookup(const QString &remote, const QString &lircButton) const
{
    // Since QHash is implicitly shared, this operation is blazing fast
    // regardless of the size of the mapping for the particular remote
    RemoteMapping mapping = m_remotes.value(remote, RemoteMapping());
    if (!mapping.isEmpty())
    {
        return mapping.value(lircButton, Navigation::Undefined);
    }

    return Navigation::Undefined;
}

bool Lircmap::parse(const QString &filepath)
{
    QFile file(filepath);
    if (file.open(QIODevice::ReadOnly))
    {
        m_reader.setDevice(&file);

        while (m_reader.readNextStartElement() && QStringRef::compare(m_reader.name(), QLatin1Literal("lircmap"), Qt::CaseInsensitive) == 0)
        {
            readLircmapElement();
            break;
        }

        return true;
    }

    return false;
}

void Lircmap::clear()
{
    m_remotes.clear();
}

void Lircmap::readLircmapElement()
{
    // Assumes current element is "lircmap"
    while (m_reader.readNextStartElement())
    {
        if (QStringRef::compare(m_reader.name(), QLatin1Literal("remote"), Qt::CaseInsensitive) == 0)
        {
            readRemoteElement();
        }
        else
        {
            qCritical() << (QStringLiteral("Lircmap.xml: Unexpected element in lircmap. Expected \"remote\", got \"")
                            + m_reader.name().toString() + QStringLiteral("\"."));
            return;
        }
    }
}

void Lircmap::readRemoteElement()
{
    QXmlStreamAttributes attributes = m_reader.attributes();
    QStringRef device = attributes.value(QLatin1Literal("device"));

    if (!device.isEmpty())
    {
        // Retrieve existing mapping or create new if necessary
        RemoteMapping & mapping = m_remotes[device.toString()];

        while (m_reader.readNextStartElement())
        {
            Navigation::Button button = translateInternalButton(m_reader.name().toString());
            QString lircName = m_reader.readElementText();

            if (m_reader.error() == QXmlStreamReader::NoError)
            {
                mapping[lircName] = button;
                m_reader.readNext();
            }
            else
            {
                qCritical("Lircmap.xml: Unexpected element inside button element.");
            }
        }
    }
    else
    {
        qWarning() << QStringLiteral("Lircmap.xml: Remote with no device detected. Skipping.");
    }
}

LircClient::LircClient(QObject *parent) :
    QObject(parent), m_delay(3), m_socket(new QLocalSocket)
{
#ifdef Q_OS_LINUX
    connect(m_socket.data(), SIGNAL(readyRead()), this, SLOT(readData()));
    connect(m_socket.data(), SIGNAL(error(QLocalSocket::LocalSocketError)),
            this, SLOT(handleError(QLocalSocket::LocalSocketError)));

    populateLircmap();
#endif
}

void LircClient::attach()
{
#ifdef Q_OS_LINUX
    m_socket->connectToServer(QString(LIRC_ADDRESS), QIODevice::ReadOnly);
#endif
}

int LircClient::delay() const
{
    return m_delay;
}

void LircClient::setDelay(int delay)
{
    if (delay >= 0)
    {
        m_delay = delay;
    }
}

void LircClient::readData()
{
    while (m_socket->bytesAvailable() > 0)
    {
        QString line = QString(m_socket->readLine()).simplified();

        QStringList parts = line.split(" ", QString::SkipEmptyParts);

        if (parts.count() == 4)
        {
            bool ok; // Holds whether the conversion to integer was successful
            int repeatCount = parts[1].toInt(&ok, 16);

            if (ok && (repeatCount == 0 || repeatCount > m_delay))
            {
                QString lircButton = parts[2];
                QString remote = parts[3];
                Navigation::Button button = m_map.lookup(remote, lircButton);

                if (button != Navigation::Undefined)
                {
                    Navigation::dispatchNavigationEvent(button, repeatCount > 0);
                }
            }
        }
        else
        {
            qDebug() << ("LIRC: Unhandled data received. Raw message: " + line);
        }
    }
}

void LircClient::handleError(QLocalSocket::LocalSocketError err)
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
        qWarning() << "LIRC: Connection closed by peer. ";
    }
    else if (err == QLocalSocket::SocketAccessError)
    {
        qWarning() << "LIRC: Socket access error. ";
    }
    else
    {
        qWarning() << "LIRC: Unknown socket error occured.";
    }
}

void LircClient::populateLircmap()
{
    m_map.clear();
    QStringList maps = getLircmapPaths();

    // Parse in reverse order, so that the last one parsed (the number one in the list)
    // will take priority over all preceding
    for (int i = maps.count() - 1; i >= 0; --i)
    {
        if (m_map.parse(maps[i]))
        {
            qDebug() << "LIRC: Successfully loaded " << maps[i];
        }
    }
}
