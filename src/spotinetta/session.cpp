#include <Spotinetta/session.h>
#include "events.h"

#include <QCoreApplication>
#include <QtAlgorithms>

namespace Spotinetta {

namespace {

// Declare callbacks
void SP_CALLCONV handleLoggedIn(sp_session *, sp_error);
void SP_CALLCONV handleLoggedOut(sp_session *);
void SP_CALLCONV handleConnectionError(sp_session *, sp_error);
void SP_CALLCONV handleConnectionStateUpdated(sp_session *);
void SP_CALLCONV handleNotifyMainThread(sp_session *);
void SP_CALLCONV handleLogMessage(sp_session *, const char *);
void SP_CALLCONV handleMetadataUpdated(sp_session *);
int  SP_CALLCONV handleMusicDelivery(sp_session *, const sp_audioformat *, const void *, int);
void SP_CALLCONV handleStreamingError(sp_session *, sp_error);
void SP_CALLCONV handleEndOfTrack(sp_session *);
//void SP_CALLCONV handleGetAudioBufferStats(sp_session *, sp_audio_buffer_stats *) { }
//void SP_CALLCONV handleStartPlayback(sp_session *) { }
//void SP_CALLCONV handleStopPlayback(sp_session *) { }

}

ApplicationKey::ApplicationKey(const uint8_t *key, size_t size)
    :   m_data(size)
{
    qCopy(key, key + size, m_data.begin());
}

QVector<uint8_t> ApplicationKey::data() const
{
    return m_data;
}

Session::Session(const SessionConfig &config, QObject *parent)
    :   QObject(parent)
{
    m_config = config;

    sp_session_config       spconfig;
    sp_session_callbacks    callbacks;
    sp_session *            session = 0;

    memset(&spconfig, 0, sizeof(sp_session_config));
    memset(&callbacks, 0, sizeof(sp_session_callbacks));

    // For header/lib compatibility check
    spconfig.api_version = SPOTIFY_API_VERSION;
    // Use the userdata field to store the address of this Session object,
    // so that we may forward events from the callbacks
    spconfig.userdata = static_cast<void *>(this);

    spconfig.cache_location = config.cacheLocation.toUtf8();
    spconfig.settings_location = config.settingsLocation.toUtf8();
    spconfig.application_key = config.applicationKey.data().constData();
    spconfig.application_key_size = config.applicationKey.data().size();
    spconfig.user_agent = config.userAgent.toUtf8();
    spconfig.device_id = config.deviceId.toUtf8();
    spconfig.proxy = config.proxy.toUtf8();
    spconfig.proxy_username = config.proxyUsername.toUtf8();
    spconfig.proxy_password = config.proxyPassword.toUtf8();
    spconfig.tracefile = config.traceFile.toUtf8();
    spconfig.compress_playlists = config.compressPlaylists;
    spconfig.dont_save_metadata_for_playlists = config.dontSaveMetadataForPlaylists;
    spconfig.initially_unload_playlists = config.initiallyUnloadPlaylists;

    callbacks.logged_in = &handleLoggedIn;
    callbacks.logged_out = &handleLoggedOut;
    callbacks.connection_error = &handleConnectionError;
    callbacks.connectionstate_updated = &handleConnectionStateUpdated;
    callbacks.log_message = &handleLogMessage;
    callbacks.notify_main_thread = &handleNotifyMainThread;
    callbacks.music_delivery = &handleMusicDelivery;
    callbacks.metadata_updated = &handleMetadataUpdated;
    callbacks.streaming_error = &handleStreamingError;
    callbacks.end_of_track = &handleEndOfTrack;
    //    callbacks.get_audio_buffer_stats = &handleGetAudioBufferStats;
    //    callbacks.start_playback = &handleStartPlayback;
    //    callbacks.stop_playback = &handleStopPlayback;

    spconfig.callbacks = &callbacks;

    m_error = static_cast<Error>(sp_session_create(&spconfig, &session));

    if (m_error == Error::Ok)
    {
        m_handle.reset(session, &sp_session_release);
    }
}

bool Session::isValid() const
{
    return m_handle.data() != 0;
}

Error Session::error() const
{
    return m_error;
}

sp_session * Session::handle() const
{
    return m_handle.data();
}

Session::ConnectionState Session::connectionState() const
{
    return isValid() ? static_cast<ConnectionState>(sp_session_connectionstate(handle())) : ConnectionState::Undefined;
}

void Session::login(const QString &username, const QString &password, bool rememberMe)
{
    if (isValid())
        sp_session_login(handle(), username.toUtf8(), password.toUtf8(), rememberMe, 0);
}

void Session::logout()
{
    if (isValid())
        sp_session_logout(handle());
}

void Session::customEvent(QEvent * e)
{
    // Only Spotinetta::Event events should be sent to Session
    Event::Type t = static_cast<Spotinetta::Event::Type>(e->type());
    Event * e = static_cast<Event *>(e);

    switch (t)
    {
    case (Event::Type::LoginEvent):
        if (e->error() == Error::Ok)
        {
            emit loggedIn();
        }
        else
        {
            // Login unsuccessful
        }
        break;
    case (Event::Type::LogoutEvent):
        emit loggedOut();
        break;
    case (Event::Type::ConnectionStateUpdatedEvent):
        emit connectionStateChanged();
        break;

    default:
        break;
    }
}

namespace {
void SP_CALLCONV handleLoggedIn(sp_session * s, sp_error e) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::LoginEvent, static_cast<Error>(e)));
}

void SP_CALLCONV handleLoggedOut(sp_session * s) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::LogoutEvent));
}

void SP_CALLCONV handleConnectionError(sp_session * s, sp_error e) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::ConnectionErrorEvent, static_cast<Error>(e)));
}

void SP_CALLCONV handleConnectionStateUpdated(sp_session * s) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::ConnectionStateUpdatedEvent));
}

void SP_CALLCONV handleNotifyMainThread(sp_session * s) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::NotifyMainThreadEvent));
}

void SP_CALLCONV handleLogMessage(sp_session * s, const char * message) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::LogEvent, Error::Ok, QByteArray(message)));
}

void SP_CALLCONV handleMetadataUpdated(sp_session * s) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::MetadataUpdatedEvent));
}

int  SP_CALLCONV handleMusicDelivery(sp_session *, const sp_audioformat *, const void *, int) {
    return 0;
}

void SP_CALLCONV handleStreamingError(sp_session * s, sp_error e) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::StreamingErrorEvent, static_cast<Error>(e)));
}

void SP_CALLCONV handleEndOfTrack(sp_session * s) {
    Session * session = static_cast<Session *>(sp_session_userdata(s));
    QCoreApplication::postEvent(session, new Event(Event::Type::EndOfTrackEvent));
}

}


}
