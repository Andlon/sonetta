#include <Sonata/spplaylistcontainerwatcher.h>

using namespace SonataPrivate;

#include "../spcore.h"
#include "../spfactory.h"

SpPlaylistContainerWatcher::SpPlaylistContainerWatcher(QObject *parent)
    :   QObject(parent)
{
}

SpPlaylistContainerWatcher::~SpPlaylistContainerWatcher()
{
    SpHandle core = SpCore::handle();
    if (m_container.isValid() && !core.isNull())
        core->factory()->unregisterContainerWatcher(this);
}

SpPlaylistContainer SpPlaylistContainerWatcher::container() const
{
    return m_container;
}

void SpPlaylistContainerWatcher::setContainer(const SpPlaylistContainer &container)
{
    if (container == m_container)
        return;

    SpHandle core = SpCore::handle();
    if (core.isNull())
        return;

    // Unregister old container
    if (m_container.isValid())
        core->factory()->unregisterContainerWatcher(this);

    // Swap containers
    m_container = container;

    // Register new container
    if (m_container.isValid())
        core->factory()->registerContainerWatcher(this);

    emit containerChanged();
    if (m_container.isLoaded())
        emit containerLoaded();
}

int SpPlaylistContainerWatcher::playlistCount() const
{
    return m_container.playlistCount();
}

SpPlaylistList SpPlaylistContainerWatcher::playlists() const
{
    return m_container.playlists();
}

SpPlaylist SpPlaylistContainerWatcher::playlistAt(int index) const
{
    return m_container.playlistAt(index);
}

bool SpPlaylistContainerWatcher::isValid() const
{
    return m_container.isValid();
}

bool SpPlaylistContainerWatcher::isLoaded() const
{
    return m_container.isLoaded();
}

bool SpPlaylistContainerWatcher::event(QEvent *event)
{
    if (event->type() == (QEvent::Type) SonataPrivate::ContainerLoadedEvent)
    {
        emit containerLoaded();
        return true;
    }
    else if (event->type() == (QEvent::Type) SonataPrivate::PlaylistMovedEvent)
    {
        SonataPrivate::ContainerModificationEvent * e = static_cast<SonataPrivate::ContainerModificationEvent *>(event);
        emit playlistMoved(e->position, e->newPosition);
    }
    else if (event->type() == (QEvent::Type) SonataPrivate::PlaylistAddedEvent)
    {
        SonataPrivate::ContainerModificationEvent * e = static_cast<SonataPrivate::ContainerModificationEvent *>(event);
        emit playlistAdded(e->position);
    }
    else if (event->type() == (QEvent::Type) SonataPrivate::PlaylistRemovedEvent)
    {
        SonataPrivate::ContainerModificationEvent * e = static_cast<SonataPrivate::ContainerModificationEvent *>(event);
        emit playlistRemoved(e->position);
    }

    return QObject::event(event);
}
