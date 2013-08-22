#include <Sonata/spplaylistwatcher.h>
#include <QVariant>

#include <QDebug>

using namespace SonataPrivate;

#include "../spcore.h"
#include "../spfactory.h"

static void enqueueEvent(QQueue<QEvent> &queue, const QEvent &event);


SpPlaylistWatcher::SpPlaylistWatcher(QObject *parent)
    :   QObject(parent), m_isUpdating(false)
{
    connect(this, SIGNAL(playlistChanged()),
            this, SIGNAL(nameChanged()));
    connect(this, SIGNAL(playlistChanged()),
            this, SIGNAL(descriptionChanged()));
    connect(this, SIGNAL(playlistChanged()),
            this, SIGNAL(imageChanged()));
}

SpPlaylistWatcher::~SpPlaylistWatcher()
{
    SpHandle core = SpCore::handle();
    if (m_playlist.isValid() && !core.isNull())
        core->factory()->unregisterPlaylistWatcher(this);
}

SpPlaylist SpPlaylistWatcher::playlist() const
{
    return m_playlist;
}

void SpPlaylistWatcher::setPlaylist(const SpPlaylist &playlist)
{
    if (playlist == m_playlist)
        return;

    SpHandle core = SpCore::handle();
    if (core.isNull())
        return;

    // Unregister old playlist
    if (m_playlist.isValid())
        core->factory()->unregisterPlaylistWatcher(this);

    // Swap playlists
    m_playlist = playlist;

    // Register new container
    if (m_playlist.isValid())
        core->factory()->registerPlaylistWatcher(this);

    emit playlistChanged();
    if (m_playlist.isLoaded())
        emit stateChanged();
}

SpTrackList SpPlaylistWatcher::tracks() const
{
    return m_playlist.tracks();
}

int SpPlaylistWatcher::trackCount() const
{
    return m_playlist.trackCount();
}

SpTrack SpPlaylistWatcher::trackAt(int index) const
{
    return m_playlist.trackAt(index);
}

QString SpPlaylistWatcher::name() const
{
    return m_playlist.name();
}

QString SpPlaylistWatcher::description() const
{
    return m_playlist.description();
}

SpImage SpPlaylistWatcher::image() const
{
    return m_playlist.image();
}

bool SpPlaylistWatcher::isValid() const
{
    return m_playlist.isValid();
}

bool SpPlaylistWatcher::isLoaded() const
{
    return m_playlist.isLoaded();
}

bool SpPlaylistWatcher::event(QEvent *event)
{
    if (event->type() == (QEvent::Type) PlaylistStateChangedEvent)
    {
        if (!m_isUpdating)
            emit stateChanged();
        else
            enqueueEvent(m_eventQueue, *event);

        return true;
    }
    else if (event->type() == (QEvent::Type) PlaylistMetadataUpdatedEvent)
    {
        if (!m_isUpdating)
            emit metadataUpdated();
        else
            enqueueEvent(m_eventQueue, *event);
        return true;
    }
    else if (event->type() == (QEvent::Type) PlaylistUpdateStartedEvent)
    {
        m_isUpdating = true;
    }
    else if (event->type() == (QEvent::Type) PlaylistUpdateFinishedEvent)
    {
        m_isUpdating = false;
        while (!m_eventQueue.isEmpty())
        {
            QEvent dequeued = m_eventQueue.dequeue();
            this->event(&dequeued);
        }
    }
    else if (event->type() == (QEvent::Type) PlaylistDescriptionChangedEvent)
    {
        if (!m_isUpdating)
            emit descriptionChanged();
        else
            enqueueEvent(m_eventQueue, *event);
    }
    else if (event->type() == (QEvent::Type) PlaylistRenamedEvent)
    {
        if (!m_isUpdating)
            emit nameChanged();
        else
            enqueueEvent(m_eventQueue, *event);
    }
    else if (event->type() == (QEvent::Type) PlaylistImageChangedEvent)
    {
        if (!m_isUpdating)
            emit imageChanged();
        else
            enqueueEvent(m_eventQueue, *event);
    }
    else if (event->type() == (QEvent::Type) PlaylistTracksAddedEvent)
    {
        PlaylistModificationEvent * plEvent = static_cast<PlaylistModificationEvent *>(event);
        emit tracksAdded(plEvent->tracks, plEvent->position);
    }
    else if (event->type() == (QEvent::Type) PlaylistTracksRemovedEvent)
    {
        PlaylistModificationEvent * plEvent = static_cast<PlaylistModificationEvent *>(event);
        emit tracksRemoved(plEvent->trackPositions);
    }
    else if (event->type() == (QEvent::Type) PlaylistTracksMovedEvent)
    {
        PlaylistModificationEvent * plEvent = static_cast<PlaylistModificationEvent *>(event);
        emit tracksMoved(plEvent->trackPositions, plEvent->position);
    }
    return QObject::event(event);
}

static void enqueueEvent(QQueue<QEvent> &queue, const QEvent &event)
{
    // Check if already exists
    foreach (QEvent enqueuedEvent, queue)
    {
        if (enqueuedEvent.type() == event.type())
            return;
    }

    queue.enqueue(event);
}
