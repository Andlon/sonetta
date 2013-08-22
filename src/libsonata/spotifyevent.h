#ifndef SPOTIFYEVENT_H
#define SPOTIFYEVENT_H

#include <QEvent>
#include <libspotify/api.h>

namespace SonataPrivate {

class SpotifyEvent : public QEvent
{
public:
    enum EventType {
        LoginEvent,
        LogoutEvent,
        ConnectionErrorEvent,
        ConnectionStateUpdatedEvent,
        MetadataUpdatedEvent,
        LogEvent,
        NotifyMainThreadEvent,
        StartPlaybackEvent,
        StopPlaybackEvent,
        EndOfTrackEvent,
        StreamingErrorEvent
    };

    SpotifyEvent(EventType type, sp_error error = SP_ERROR_OK, QByteArray data = QByteArray());

    static QEvent::Type qType();

    EventType   eventType() const;
    sp_error    error() const;
    QByteArray  data() const;
    
signals:
    
public slots:

private:
    QByteArray m_data;
    sp_error m_error;
    EventType m_type;
    
};




// Implementation

inline SpotifyEvent::SpotifyEvent(EventType type, sp_error error, QByteArray data)
    :   QEvent(SpotifyEvent::qType()), m_data(data), m_error(error), m_type(type)
{

}

inline SpotifyEvent::EventType SpotifyEvent::eventType() const
{
    return m_type;
}

inline sp_error SpotifyEvent::error() const
{
    return m_error;
}

inline QByteArray SpotifyEvent::data() const
{
    return m_data;
}

inline QEvent::Type SpotifyEvent::qType()
{
    static QEvent::Type t = (QEvent::Type) QEvent::registerEventType(10000);
    return t;
}

}

#endif // SPOTIFYEVENT_H
