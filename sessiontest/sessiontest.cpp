#include "sessiontest.h"
#include "appkey.c"

#include <QDebug>

// Declare callbacks
static sp_session_callbacks callbacks;

static void SP_CALLCONV handleLoggedIn(sp_session *, sp_error);
static void SP_CALLCONV handleLoggedOut(sp_session *);
static void SP_CALLCONV handleNotifyMainThread(sp_session *);
static void SP_CALLCONV handleLogMessage(sp_session *, const char *);
static void SP_CALLCONV handleConnectionError(sp_session *, sp_error);

SessionTest::SessionTest(QObject * parent)
    :   QObject(parent), m_session(0), m_timerId(0)
{
    m_logoutTimer = new QTimer(this);
    m_logoutTimer->setInterval(10000);
    m_logoutTimer->setSingleShot(true);
    connect(m_logoutTimer, SIGNAL(timeout()), this,
            SLOT(logout()));
}

bool SessionTest::createSession()
{
    // Prepare session creation
    sp_session_config config;
    sp_error error;
    sp_session * session;

    memset(&config, 0, sizeof(config));
    memset(&callbacks, 0, sizeof(callbacks));

    config.api_version = SPOTIFY_API_VERSION;

    // Set appropriate paths for your system
    config.cache_location = "D:/spotify";
    config.settings_location = "D:/spotify";

    config.application_key = g_appkey;
    config.application_key_size = g_appkey_size;
    config.user_agent ="SessionTest";


    callbacks.metadata_updated = 0;
    callbacks.message_to_user = 0;
    callbacks.music_delivery = 0;
    callbacks.play_token_lost = 0;
    callbacks.end_of_track = 0;
    callbacks.streaming_error = 0;
    callbacks.userinfo_updated = 0;
    callbacks.start_playback = 0;
    callbacks.stop_playback = 0;
    callbacks.get_audio_buffer_stats = 0;
    callbacks.offline_status_updated = 0;

    callbacks.logged_in = &handleLoggedIn;
    callbacks.logged_out = &handleLoggedOut;
    callbacks.log_message = &handleLogMessage;
    callbacks.notify_main_thread = &handleNotifyMainThread;
    callbacks.connection_error = &handleConnectionError;

    config.callbacks = &callbacks;

    error = sp_session_create(&config, &session);
    if (error == SP_ERROR_OK)
    {
        m_session = session;
        return true;
    }
    else
    {
        qWarning() << "Error when creating session: " << sp_error_message(error);
        return false;
    }
}

void SessionTest::login(const QString &username, const QString &password)
{
    sp_session_login(m_session, username.toUtf8(), password.toUtf8(), false, 0);
}

void SessionTest::logout()
{
    if (m_session)
    {
        qDebug() << "Attempting to logout.";
        sp_session_logout(m_session);
    }
    else
    {
        qWarning() << "Attempted to logout without session.";
    }
}

SessionTest * SessionTest::instance()
{
    static SessionTest * instance = 0;

    if (instance == 0)
        instance = new SessionTest();

    return instance;
}

void SessionTest::processSpotifyEvents()
{
    if (m_session == 0)
    {
        qDebug() << "Process: No session.";
        return;
    }

    int interval = 0;
    sp_session_process_events(m_session, &interval);
    qDebug() << interval;
    m_timerId = startTimer(interval);
}

bool SessionTest::event(QEvent *event)
{
    QEvent::Type type = event->type();

    if (type == QEvent::Timer)
    {
        QTimerEvent * timerEvent = static_cast<QTimerEvent *>(event);

        if (timerEvent->timerId() == m_timerId)
            processSpotifyEvents();

        return true;
    }

    switch ((int) type)
    {
    case ((int) SpLoggedInEvent):
        // Login complete, attempt logout
        m_logoutTimer->start();
        return true;
        break;
    case ((int) SpLoggedOutEvent):
        // Logout complete, attempt session release
        sp_session_release(m_session);
        m_session = 0;
        qDebug() << "Session released.";
        QCoreApplication::exit(10);
        return true;
        break;
    case ((int) SpNotifyMainThreadEvent):
        killTimer(m_timerId);
        processSpotifyEvents();
        return true;
        break;

    default:
        return QObject::event(event);
        break;
    }
}

static void SP_CALLCONV handleLoggedIn(sp_session *, sp_error error)
{
    qDebug() << "Logged in. Error:" << sp_error_message(error);
    SessionTest * s = SessionTest::instance();
    QCoreApplication::postEvent(s, new QEvent((QEvent::Type) SpLoggedInEvent));
}

static void SP_CALLCONV handleLoggedOut(sp_session *)
{
    qDebug() << "Got logout callback.";
    SessionTest * s = SessionTest::instance();
    QCoreApplication::postEvent(s, new QEvent((QEvent::Type) SpLoggedOutEvent));
}

static void SP_CALLCONV handleNotifyMainThread(sp_session *)
{
    SessionTest * s = SessionTest::instance();
    QCoreApplication::postEvent(s, new QEvent((QEvent::Type) SpNotifyMainThreadEvent));
}

static void SP_CALLCONV handleLogMessage(sp_session *, const char *message)
{
    qDebug() << "Spotify: " << message;
}

static void SP_CALLCONV handleConnectionError(sp_session *, sp_error error)
{
    qDebug() << "Connection error: " << sp_error_message(error);
}
