#include "spcore.h"

#include "output/spdefaultoutput.h"

#include <QCoreApplication>

#include <QDebug>

using namespace SonataPrivate;

QWeakPointer<SpCore> SpCore::s_instance;

// Declare callbacks
static void SP_CALLCONV handleLoggedIn(sp_session *, sp_error);
static void SP_CALLCONV handleLoggedOut(sp_session *);
static void SP_CALLCONV handleConnectionError(sp_session *, sp_error);
static void SP_CALLCONV handleConnectionStateUpdated(sp_session *);
static void SP_CALLCONV handleNotifyMainThread(sp_session *);
static void SP_CALLCONV handleLogMessage(sp_session *, const char *);
static void SP_CALLCONV handleMetadataUpdated(sp_session *);
static int  SP_CALLCONV handleMusicDelivery(sp_session *, const sp_audioformat *, const void *, int);
static void SP_CALLCONV handleStreamingError(sp_session *, sp_error);
static void SP_CALLCONV handleEndOfTrack(sp_session *);
static void SP_CALLCONV handleGetAudioBufferStats(sp_session *, sp_audio_buffer_stats *stats);
static void SP_CALLCONV handleStartPlayback(sp_session *) { }
static void SP_CALLCONV handleStopPlayback(sp_session *) { }

static void doNothing(sp_session *) { }

SpCore::SpCore(const uint8_t *key, size_t size)
    :   QObject(0), m_keySize(size), m_state(Spotify::UndefinedState),
      m_output(new SpDefaultOutput()), m_factory(new SpFactory(this)), m_timerId(0),
      m_audioThread(new QThread()), m_pollTimer(new QTimer(this))
{
    // Copy key
    m_key = new uint8_t[size];
    for (size_t i = 0; i < size; ++i)
        *(m_key + i) = *(key + i);

    m_pollTimer->setSingleShot(false);
    m_pollTimer->start(250);

    connect(m_pollTimer, SIGNAL(timeout()), this, SLOT(handlePolling()));

    m_output->moveToThread(m_audioThread);

    setupOutputConnections();
}

SpCore::~SpCore()
{
    if (connectionState() == Spotify::LoggedInState)
        logout();

    // Cleanup of libspotify-related objects
    releaseSession();

    delete[] m_key;

    // Clean up audio thread
    if (m_audioThread->isRunning())
    {
        connect(m_output, SIGNAL(destroyed()), m_audioThread, SLOT(quit()));
        connect(m_audioThread, SIGNAL(finished()), m_audioThread, SLOT(deleteLater()));
    }
    else
    {
        m_audioThread->deleteLater();
    }

    m_output->deleteLater();

    // Perform proper cleanup here
}

SpCore::ConnectionState SpCore::connectionState() const
{
    return m_state;
}

SpCore::PlaybackState SpCore::playbackState() const
{
    return m_output->state();
}

SpHandle SpCore::createHandle(const uint8_t *key, size_t size)
{
    SpHandle handle = SpCore::s_instance.toStrongRef();

    if (!handle.isNull())
    {
#ifdef QT_DEBUG
        qWarning() << "Attempted to create SpCore handle when handle already exists.";
#endif
    }
    else
    {
        handle.reset(new SpCore(key, size), &SpCore::deleteLater);
        SpCore::s_instance = QWeakPointer<SpCore>(handle);
    }

    return handle;
}

SpHandle SpCore::handle()
{
    return SpCore::s_instance.toStrongRef();
}

bool SpCore::sessionIsValid() const
{
    return !m_session.isNull();
}

QWeakPointer<sp_session> SpCore::session() const
{
    return m_session.toWeakRef();
}

bool SpCore::createSession()
{
    if (sessionIsValid())
        return false;

    // Prepare session creation
    sp_session_config config;
    sp_session_callbacks callbacks;
    sp_error error;

    sp_session * session;

    memset(&config, 0, sizeof(sp_session_config));
    memset(&callbacks, 0, sizeof(sp_session_callbacks));

    config.api_version = SPOTIFY_API_VERSION;
    config.cache_location = "C:\\sonata";
    config.settings_location = "C:\\sonata";

    config.application_key = m_key;
    config.application_key_size = m_keySize;
    config.user_agent ="Sonata";

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

    config.callbacks = &callbacks;

    error = sp_session_create(&config, &session);
    if (error == SP_ERROR_OK)
    {
        m_session.reset(session, &doNothing);
        m_state = (Spotify::ConnectionState) sp_session_connectionstate(m_session.data());
        return true;
    }
    else
    {
        qWarning() << "Error when creating session: " << sp_error_message(error);
        return false;
    }
}

void SpCore::releaseSession()
{
    if (!sessionIsValid())
        return;

    sp_session_release(m_session.data());
    m_session.clear();
}

void SpCore::processSpotifyEvents()
{
    int interval = 0;

    do {
    sp_session_process_events(m_session.data(), &interval);
    } while (interval == 0);

    m_timerId = startTimer(interval);
}

bool SpCore::event(QEvent *event)
{
    if (event->type() == SpotifyEvent::qType())
        return spotifyEvent(static_cast<SpotifyEvent *>(event));

    return QObject::event(event);
}

bool SpCore::spotifyEvent(SpotifyEvent *event)
{
    SpotifyEvent::EventType type = event->eventType();

    switch (type)
    {
    case SpotifyEvent::NotifyMainThreadEvent:
        killTimer(m_timerId);
        processSpotifyEvents();
        break;
    case SpotifyEvent::LoginEvent:
        if (event->error() == SP_ERROR_OK)
        {
            // This is a workaround for a libspotify bug where logging out causes a crash,
            // unless the rootcontainer exists. Keeping the root container
            // a member variable ensures it exists as long as we're logged in.
            sp_playlistcontainer * handle = sp_session_playlistcontainer(m_session.data());
            m_rootContainer = factory()->createPlaylistContainer(handle);
            emit loggedIn();
        }
        else
            // Do something here, possibly loginFailed signal
            qWarning() << "Login failed. Error: " << sp_error_message(event->error());

        break;
    case SpotifyEvent::LogoutEvent:
        m_rootContainer = SpPlaylistContainer();
        m_currentTrack = SpTrack();
        emit currentTrackChanged();
        emit loggedOut();
        break;
    case SpotifyEvent::LogEvent:
        // Note: This should be pushed to a signal eventually
        qDebug() << event->data();
        break;
    case SpotifyEvent::MetadataUpdatedEvent:
        factory()->updateMeta();
        break;
    case SpotifyEvent::EndOfTrackEvent:
        processEndOfTrack();
        break;
    case SpotifyEvent::ConnectionStateUpdatedEvent:
        if (sessionIsValid())
        {
            m_state = (Spotify::ConnectionState) sp_session_connectionstate(m_session.data());
            emit connectionStateChanged();
        }
        break;
    case SpotifyEvent::StreamingErrorEvent:
        qDebug() << "Streaming error: " << sp_error_message(event->error());
        break;

    default:
        return true;
    }

    return true;
}

SpAudioOutput * SpCore::audioOutput() const
{
    return m_output;
}

SpFactory * SpCore::factory() const
{
    return m_factory;
}

int SpCore::position() const
{
    return m_output->position();
}

void SpCore::setAudioOutput(SpAudioOutput *output)
{
    // Only allow setting output before session is created
    if (sessionIsValid())
        return;

    if (output == 0)
    {
        if (m_output)
            m_output->deleteLater();

        // Assign default device
        m_output = new SpDefaultOutput(this);
        setupOutputConnections();
    }
    else if (output != m_output)
    {
        if (m_output)
            m_output->deleteLater();

        // Assign device
        m_output = output;
        setupOutputConnections();
    }
}

void SpCore::setupOutputConnections()
{
    connect(m_output, SIGNAL(stateChanged()),
            this, SIGNAL(playbackStateChanged()));
}

void SpCore::login(const QString &username, const QString &password)
{
    if (!sessionIsValid())
    {
        qWarning() << "Sonata: Attempted to login to Spotify without session.";
        return;
    }

    sp_session_login(m_session.data(), username.toUtf8(), password.toUtf8(), false, 0);
}

void SpCore::logout()
{
    if (!sessionIsValid())
    {
        qWarning() << "Sonata: Attempted to logout from Spotify without session.";
        return;
    }

    sp_session_logout(m_session.data());
}

bool SpCore::play(const SpTrack &track)
{
    if (!sessionIsValid())
        return false;

    if (!track.isLoaded())
        return false;

    SpTrack playable = track.playableTrack();

    sp_track * handle = m_factory->getTrackHandle(playable);
    if (!handle)
        return false;

    sp_error error = sp_session_player_load(m_session.data(), handle);

    if (error == SP_ERROR_OK)
    {
        if (!m_audioThread->isRunning())
            m_audioThread->start();

        if (playbackState() == Spotify::PausedState)
            m_output->reset();
        else if (playbackState() == Spotify::StoppedState)
            m_output->reset();

        m_output->resetPosition(0);
        m_output->play();
        sp_session_player_play(m_session.data(), true);
        m_currentTrack = playable;
        emit currentTrackChanged();
        return true;
    }
    else
    {
        stop();
        // State ???
        qDebug() << "Error when loading track: " << sp_error_message(error);
        return false;
    }
}

bool SpCore::stop()
{
    // Should print warning here
    if (!sessionIsValid())
        return false;

    if (playbackState() == Spotify::StoppedState)
        return false;

    m_output->stop();
    m_output->resetPosition(0);

    sp_session_player_play(m_session.data(), false);
    sp_session_player_unload(m_session.data());

    m_output->reset();

    m_currentTrack = SpTrack();
    emit currentTrackChanged();

    return true;
}

bool SpCore::play()
{
    if (!sessionIsValid())
    {
        qWarning() << "Sonata: Attempted to start playback without session.";
        return false;
    }
    else if (!currentTrack().isValid())
    {
        qWarning() << "Sonata: Tried to start playbakc with invalid track.";
        return false;
    }

    if (playbackState() == Spotify::PlayState || playbackState() == Spotify::StoppedState)
        return false;

    sp_session_player_play(m_session.data(), true);

    m_output->play();

    return true;
}

bool SpCore::pause()
{
    if (!sessionIsValid())
    {
        qWarning() << "Sonata: Attempted to pause playback without session.";
        return false;
    }
    else if (!currentTrack().isValid())
    {
        qWarning() << "Sonata: Tried to pause playback with invalid track.";
        return false;
    }

    if (playbackState() == Spotify::PausedState)
        return false;


    sp_session_player_play(m_session.data(), false);

    m_output->pause();

    return true;
}

void SpCore::seek(int offset)
{
    if (!sessionIsValid())
    {
        qWarning() << "Sonata: Attempted to seek playback without session.";
        return;
    }

    if (!currentTrack().isValid())
        return;

    qDebug() << "Seeking to " << offset;

    sp_session_player_seek(m_session.data(), offset);
    m_output->resetPosition(offset);
}

SpTrack SpCore::currentTrack() const
{
    return m_currentTrack;
}

void SpCore::handlePolling()
{
//    if (playbackState() == Spotify::PlayState && currentTrack().isLoaded() && position() >= currentTrack().duration())
//        processEndOfTrack();
    emit positionUpdated();
}

void SpCore::processEndOfTrack()
{
    m_currentTrack = SpTrack();
    emit endOfTrack();
    emit currentTrackChanged();

    if (m_currentTrack.isValid())
    {
        // Reset position, fix accuracy of this later
        m_output->resetPosition(0);
    }
    else
    {
        // Stop playback if no other track has been played
        qDebug() << "Current track invalid. Stopping...";
        stop();
    }
}

SpPlaylistContainer SpCore::rootContainer() const
{
    return m_rootContainer;
}

















static void postSpotifyEvent(QEvent * event)
{
    SpHandle handle = SpCore::handle();
    if (!handle.isNull())
        QCoreApplication::postEvent(handle.data(), event);
    else
        delete event;
}

static bool sendSpotifyEvent(QEvent * event)
{
    SpHandle handle = SpCore::handle();
    if (!handle.isNull())
    {
        return QCoreApplication::sendEvent(handle.data(), event);   
    }

    return false;
}


static void SP_CALLCONV handleLoggedIn(sp_session *, sp_error error)
{
    SpotifyEvent * event = new SpotifyEvent(SpotifyEvent::LoginEvent, error);
    postSpotifyEvent(event);
}

static void SP_CALLCONV handleLoggedOut(sp_session *)
{
    SpotifyEvent * event = new SpotifyEvent(SpotifyEvent::LogoutEvent);
    postSpotifyEvent(event);
}

static void SP_CALLCONV handleConnectionError(sp_session *, sp_error error)
{
    SpotifyEvent event(SpotifyEvent::ConnectionErrorEvent, error);
    sendSpotifyEvent(&event);
}

static void SP_CALLCONV handleConnectionStateUpdated(sp_session *)
{
    SpotifyEvent * event = new SpotifyEvent(SpotifyEvent::ConnectionStateUpdatedEvent);
    postSpotifyEvent(event);
}

static void SP_CALLCONV handleNotifyMainThread(sp_session *)
{
    SpotifyEvent * event = new SpotifyEvent(SpotifyEvent::NotifyMainThreadEvent);
    postSpotifyEvent(event);
}

static void SP_CALLCONV handleLogMessage(sp_session *, const char *data)
{
    QByteArray message(data);
    SpotifyEvent * event = new SpotifyEvent(SpotifyEvent::LogEvent, SP_ERROR_OK, message);
    postSpotifyEvent(event);
}

static void SP_CALLCONV handleMetadataUpdated(sp_session *)
{
    SpotifyEvent * event = new SpotifyEvent(SpotifyEvent::MetadataUpdatedEvent);
    postSpotifyEvent(event);
}

static int SP_CALLCONV handleMusicDelivery(sp_session *, const sp_audioformat *format, const void *frames, int num_frames)
{
    if (format->sample_type != SP_SAMPLETYPE_INT16_NATIVE_ENDIAN)
        return 0;

    SpHandle handle = SpCore::handle();
    if (handle.isNull())
        return 0;

    SpAudioOutput * output = handle->audioOutput();

    int channels = format->channels;
    int sampleRate = format->sample_rate;

    QAudioFormat audioFormat;
    audioFormat.setChannelCount(channels);
    audioFormat.setSampleRate(sampleRate);
    audioFormat.setSampleType(QAudioFormat::SignedInt);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec("audio/pcm");

    int delivered = output->deliverAudio(audioFormat, frames, num_frames);
    return delivered;
}

static void SP_CALLCONV handleEndOfTrack(sp_session *)
{
    SpotifyEvent * event = new SpotifyEvent(SpotifyEvent::EndOfTrackEvent);
    postSpotifyEvent(event);
}

static void SP_CALLCONV handleStreamingError(sp_session *, sp_error error)
{
    Q_ASSERT(QThread::currentThread() != SpCore::handle()->thread());
    SpotifyEvent event(SpotifyEvent::StreamingErrorEvent, error);
    sendSpotifyEvent(&event);
}

static void SP_CALLCONV handleGetAudioBufferStats(sp_session *, sp_audio_buffer_stats *stats)
{
    SpHandle core = SpCore::handle();
    if (!core.isNull())
        stats->samples = core->audioOutput()->bytesInBuffer();
    stats->stutter = 0;
}

