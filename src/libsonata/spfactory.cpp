#include "spfactory.h"

#include <Sonata/SpTrack>
#include <Sonata/SpLink>

#include <QCoreApplication>
#include <QDebug>
#include <QList>

#include "spcore.h"

using namespace SonataPrivate;

// Playlist callbacks
static void SP_CALLCONV handlePlaylistMetadataUpdated(sp_playlist * pl, void * userdata);
static void SP_CALLCONV handlePlaylistStateChanged(sp_playlist * pl, void *userdata);
static void SP_CALLCONV handlePlaylistUpdateInProgress(sp_playlist *pl, bool done, void *userdata);
static void SP_CALLCONV handleDescriptionChanged(sp_playlist *pl, const char *desc, void *userdata);
static void SP_CALLCONV handlePlaylistRenamed(sp_playlist *pl, void *userdata);
static void SP_CALLCONV handleImageChanged(sp_playlist *pl, const byte *image, void *userdata);
static void SP_CALLCONV handleTracksAdded(sp_playlist *pl, sp_track *const *tracks, int num_tracks, int position, void *userdata);
static void SP_CALLCONV handleTracksRemoved(sp_playlist *pl, const int *tracks, int num_tracks, void *userdata);
static void SP_CALLCONV handleTracksMoved(sp_playlist *pl, const int *tracks, int num_tracks, int new_position, void *userdata);

// PlaylistContainer callbacks
static void SP_CALLCONV handlePlaylistContainerLoaded(sp_playlistcontainer * pc, void * userdata);
static void SP_CALLCONV handlePlaylistAdded(sp_playlistcontainer *pc, sp_playlist *playlist, int position, void *userdata);
static void SP_CALLCONV handlePlaylistRemoved(sp_playlistcontainer *pc, sp_playlist *playlist, int position, void *userdata);
static void SP_CALLCONV handlePlaylistMoved(sp_playlistcontainer *pc, sp_playlist *playlist, int position, int new_position, void *userdata);

// Image callbacks
static void SP_CALLCONV handleImageLoaded(sp_image *image, void *userdata);

// Artist browse callbacks
static void SP_CALLCONV handleArtistBrowseComplete(sp_artistbrowse *result, void *userdata);

// Album browse callbacks
static void SP_CALLCONV handleAlbumBrowseComplete(sp_albumbrowse *result, void *userdata);

// Search callbacks
static void SP_CALLCONV handleSearchComplete(sp_search *result, void *userdata);

SonataPrivate::SpFactory::SpFactory(QObject *parent) :
    QObject(parent)
{
    memset(&m_playlistCallbacks, 0, sizeof(sp_playlist_callbacks));
    memset(&m_playlistContainerCallbacks, 0, sizeof(sp_playlistcontainer_callbacks));

    m_playlistCallbacks.playlist_metadata_updated = &handlePlaylistMetadataUpdated;
    m_playlistCallbacks.playlist_state_changed = &handlePlaylistStateChanged;
    m_playlistCallbacks.playlist_update_in_progress = &handlePlaylistUpdateInProgress;
    m_playlistCallbacks.playlist_renamed = &handlePlaylistRenamed;
    m_playlistCallbacks.description_changed = &handleDescriptionChanged;
    m_playlistCallbacks.image_changed = &handleImageChanged;
    m_playlistCallbacks.tracks_added = &handleTracksAdded;
    m_playlistCallbacks.tracks_removed = &handleTracksRemoved;
    m_playlistCallbacks.tracks_moved = &handleTracksMoved;

    m_playlistContainerCallbacks.container_loaded = &handlePlaylistContainerLoaded;
    m_playlistContainerCallbacks.playlist_added = &handlePlaylistAdded;
    m_playlistContainerCallbacks.playlist_removed = &handlePlaylistRemoved;
    m_playlistContainerCallbacks.playlist_moved = &handlePlaylistMoved;
}

SpTrack SonataPrivate::SpFactory::createTrack(sp_track * handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpTrack();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpTrack();

    SpTrackPrivate * priv = new SpTrackPrivate();
    priv->handle = handle;
    priv->session = session;

    bool loaded = sp_track_is_loaded(handle);
    SpTrack track(priv);
    if (!loaded)
        m_pendingTracks.append(track);

    return track;
}

SpArtist SonataPrivate::SpFactory::createArtist(sp_artist *handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpArtist();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpArtist();

    SpArtistPrivate * priv = new SpArtistPrivate();
    priv->handle = handle;
    priv->session = session;

    bool loaded = sp_artist_is_loaded(handle);
    SpArtist artist(priv);
    if (!loaded)
        m_pendingArtists.append(artist);

    return artist;
}

SpAlbum SonataPrivate::SpFactory::createAlbum(sp_album *handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpAlbum();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpAlbum();

    SpAlbumPrivate * priv = new SpAlbumPrivate();
    priv->handle = handle;
    priv->session = session;

    bool loaded = sp_album_is_loaded(handle);
    SpAlbum album(priv);
    if (!loaded)
    {
        m_pendingAlbums.append(album);
    }

    return album;
}

SpPlaylist SonataPrivate::SpFactory::createPlaylist(sp_playlist *handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpPlaylist();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpPlaylist();

    SpPlaylistPrivate * priv = new SpPlaylistPrivate();
    priv->handle = handle;
    priv->session = session;

    return SpPlaylist(priv);
}

SpPlaylistContainer SonataPrivate::SpFactory::createPlaylistContainer(sp_playlistcontainer *handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpPlaylistContainer();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpPlaylistContainer();

    SpPlaylistContainerPrivate * priv = new SpPlaylistContainerPrivate();
    priv->handle = handle;
    priv->session = session;

    return SpPlaylistContainer(priv);
}

SpImage SonataPrivate::SpFactory::createImage(sp_image *handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpImage();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpImage();

    SpImagePrivate * priv = new SpImagePrivate;
    priv->handle = handle;
    priv->session = session;

    return SpImage(priv);
}

SpArtistBrowse SonataPrivate::SpFactory::createArtistBrowse(sp_artistbrowse *handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpArtistBrowse();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpArtistBrowse();

    SpArtistBrowsePrivate * priv = new SpArtistBrowsePrivate;
    priv->handle = handle;
    priv->session = session;

    return SpArtistBrowse(priv);
}

SpAlbumBrowse SonataPrivate::SpFactory::createAlbumBrowse(sp_albumbrowse *handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpAlbumBrowse();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpAlbumBrowse();

    SpAlbumBrowsePrivate * priv = new SpAlbumBrowsePrivate;
    priv->handle = handle;
    priv->session = session;

    return SpAlbumBrowse(priv);
}

SpSearch SonataPrivate::SpFactory::createSearch(sp_search *handle) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || handle == 0)
        return SpSearch();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpSearch();

    SpSearchPrivate * priv = new SpSearchPrivate;
    priv->handle = handle;
    priv->session = session;

    return SpSearch(priv);
}

SpImage SonataPrivate::SpFactory::createImageFromId(const byte *id) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || id == 0)
        return SpImage();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpImage();

    sp_image * handle = sp_image_create(session.data(), id);
    if (!handle)
        return SpImage();

    SpImagePrivate * priv = new SpImagePrivate;
    priv->handle = handle;
    priv->session = session;

    return SpImage(priv);
}

SpLink SonataPrivate::SpFactory::createLink(sp_link *link) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || link == 0)
        return SpLink();

    SessionHandle session = core->session();
    if (session.isNull())
        return SpLink();

    SpLinkPrivate * priv = new SpLinkPrivate();
    priv->handle = link;
    priv->session = session;
    return SpLink(priv);
}

SpTrack SonataPrivate::SpFactory::createTrackFromLink(const SpLink &link) const
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || !core->sessionIsValid() || !link.isValid())
        return SpTrack();

    // Assumes link is valid and doesn't already have a track
    sp_track * track = sp_link_as_track(link.d->handle);

    if (track == 0)
        return SpTrack();

    return createTrack(track);
}

sp_track * SonataPrivate::SpFactory::getTrackHandle(const SpTrack &track) const
{
    return track.isValid() ? track.d->handle : 0;
}

sp_image * SonataPrivate::SpFactory::getImageHandle(const SpImage &image) const
{
    return image.isValid() ? image.d->handle : 0;
}

void SonataPrivate::SpFactory::updateMeta()
{
    SpHandle core = SpCore::handle();
    if (core.isNull() || !core->sessionIsValid())
        return;

    QVector<SpTrack>::Iterator tr = m_pendingTracks.begin();
    QVector<SpArtist>::Iterator ar = m_pendingArtists.begin();
    QVector<SpAlbum>::Iterator al = m_pendingAlbums.begin();

    // Tracks
    while (tr != m_pendingTracks.end())
    {
        SpTrackPrivate * priv = tr->d;
        bool loaded = sp_track_is_loaded(priv->handle);
        if (loaded)
        {
            emit trackUpdated(*tr);
            tr = m_pendingTracks.erase(tr);
        }
        else
            ++tr;
    }

    // Artists
    while (ar != m_pendingArtists.end())
    {
        SpArtistPrivate * priv = ar->d;
        bool loaded = sp_artist_is_loaded(priv->handle);
        if (loaded)
        {
            emit artistUpdated(*ar);
            ar = m_pendingArtists.erase(ar);
        }
        else
            ++ar;
    }

    // Albums
    while (al != m_pendingAlbums.end())
    {
        SpAlbumPrivate * priv = al->d;
        bool loaded = sp_album_is_loaded(priv->handle);
        if (loaded)
        {
            emit albumUpdated(*al);
            al = m_pendingAlbums.erase(al);
        }
        else
            ++al;
    }
}

void SonataPrivate::SpFactory::registerPlaylistWatcher(SpPlaylistWatcher *watcher)
{

    SpPlaylist playlist = watcher->playlist();

    if (playlist.isValid())
    {
        sp_playlist * handle = playlist.d->handle;
        sp_playlist_add_callbacks(handle, &m_playlistCallbacks, (void *) watcher);
        foreach (PlaylistCount count, m_registeredPlaylists)
        {
            if (count.handle == handle)
            {
                count.watchers.append(watcher);
                return;
            }
        }

        // Not found, add new
        PlaylistCount count;
        count.handle = handle;
        count.watchers.append(watcher);
        m_registeredPlaylists.append(count);
    }
}

void SonataPrivate::SpFactory::registerContainerWatcher(SpPlaylistContainerWatcher *watcher)
{
    SpPlaylistContainer container = watcher->container();

    if (container.isValid())
    {
        sp_playlistcontainer * handle = container.d->handle;
        sp_playlistcontainer_add_callbacks(handle, &m_playlistContainerCallbacks, (void *) watcher);
        foreach (PlaylistContainerCount count, m_registeredContainers)
        {
            if (count.handle == handle)
            {
                count.watchers.append(watcher);
                return;
            }
        }

        // Not found, add new
        PlaylistContainerCount count;
        count.handle = handle;
        count.watchers.append(watcher);
        m_registeredContainers.append(count);
    }
}

void SonataPrivate::SpFactory::unregisterPlaylistWatcher(SpPlaylistWatcher *watcher)
{

    SpPlaylist playlist = watcher->playlist();
    if (playlist.isValid())
    {
        sp_playlist * handle = playlist.d->handle;
        sp_playlist_remove_callbacks(handle, &m_playlistCallbacks, (void *) watcher);

        // Remove watcher from registry. Assume it has only been registered once (if not there's a bug in the watcher code)
        foreach (PlaylistCount count, m_registeredPlaylists)
        {
            if (count.handle == handle)
            {
                QVector<SpPlaylistWatcher *>::Iterator i = count.watchers.begin();

                while (i != count.watchers.end())
                {
                    if ((*i) == watcher)
                    {
                        count.watchers.erase(i);
                        return;
                    }
                    ++i;
                }
            }
        }
    }
}

void SonataPrivate::SpFactory::unregisterContainerWatcher(SpPlaylistContainerWatcher *watcher)
{
    SpPlaylistContainer container = watcher->container();
    if (container.isValid())
    {
        sp_playlistcontainer * handle = container.d->handle;
        sp_playlistcontainer_remove_callbacks(handle, &m_playlistContainerCallbacks, (void *) watcher);

        // Remove watcher from registry. Assume it has only been registered once (if not there's a bug in the watcher code)
        foreach (PlaylistContainerCount count, m_registeredContainers)
        {
            if (count.handle == handle)
            {
                QVector<SpPlaylistContainerWatcher *>::Iterator i = count.watchers.begin();

                while (i != count.watchers.end())
                {
                    if ((*i) == watcher)
                    {
                        count.watchers.erase(i);
                        return;
                    }
                    ++i;
                }
            }
        }
    }
}

void SonataPrivate::SpFactory::registerImageWatcher(SpImageWatcher * watcher)
{
    SpImage image = watcher->image();
    if (image.isValid())
    {
        sp_image * handle = image.d->handle;
        sp_image_add_load_callback(handle, &handleImageLoaded, static_cast<void *>(watcher));

        foreach (ImageCount count, m_registeredImages)
        {
            if (count.handle == handle)
            {
                count.watchers.append(watcher);
                return;
            }
        }

        // Not found, add new
        ImageCount count;
        count.handle = handle;
        count.watchers.append(watcher);
        m_registeredImages.append(count);
    }
}

void SonataPrivate::SpFactory::unregisterImageWatcher(SpImageWatcher * watcher)
{
    SpImage image = watcher->image();
    if (image.isValid())
    {
        sp_image * handle = image.d->handle;
        sp_image_remove_load_callback(handle, &handleImageLoaded, static_cast<void *>(watcher));
        // Remove watcher from registry. Assume it has only been registered once (if not there's a bug in the watcher code)
        foreach (ImageCount count, m_registeredImages)
        {
            if (count.handle == handle)
            {
                QVector<SpImageWatcher *>::Iterator i = count.watchers.begin();

                while (i != count.watchers.end())
                {
                    if ((*i) == watcher)
                    {
                        count.watchers.erase(i);
                        // Should really remove count as well if no more watchers are present, fix later
                        return;
                    }
                    ++i;
                }
            }
        }
    }
}

SpArtistBrowse SonataPrivate::SpFactory::browse(const SpArtist &artist, Spotify::ArtistBrowseType type) const
{
    if (artist.isValid())
    {
        sp_artist * handle = artist.d->handle;
        sp_session * session = artist.d->session.data();

        sp_artistbrowse_type t = SP_ARTISTBROWSE_NO_TRACKS;

        if (type == Spotify::ArtistBrowseNoAlbums)
            t = SP_ARTISTBROWSE_NO_ALBUMS;

        sp_artistbrowse * browse = sp_artistbrowse_create(session, handle, t, &handleArtistBrowseComplete, 0);
        return createArtistBrowse(browse);
    }

    return SpArtistBrowse();
}

SpAlbumBrowse SonataPrivate::SpFactory::browse(const SpAlbum &album)
{
    if (album.isValid())
    {
        sp_album * handle = album.d->handle;
        sp_session * session = album.d->session.data();
        sp_albumbrowse * browse = sp_albumbrowse_create(session, handle, &handleAlbumBrowseComplete, 0);
        return createAlbumBrowse(browse);
    }

    return SpAlbumBrowse();
}

SpSearch SonataPrivate::SpFactory::search(const QString &query, int trackOffset, int maxTracks, int albumOffset, int maxAlbums, int artistOffset, int maxArtists, int playlistOffset, int maxPlaylists, Spotify::SearchType type)
{
    SpHandle core = SpCore::handle();

    if (!core.isNull())
    {
        sp_search * handle = sp_search_create(core->session().data(), query.toUtf8(),
                                              trackOffset, maxTracks,
                                              albumOffset, maxAlbums,
                                              artistOffset, maxArtists,
                                              playlistOffset, maxPlaylists,
                                              SP_SEARCH_STANDARD, &handleSearchComplete, 0);

        qDebug() << "Query: " << query << " Handle: " << handle;
        return createSearch(handle);
    }

    return SpSearch();
}

void SonataPrivate::SpFactory::signalArtistBrowseCompleted(const SpArtistBrowse &browse)
{
    emit artistBrowseCompleted(browse);
}

void SonataPrivate::SpFactory::signalAlbumBrowseCompleted(const SpAlbumBrowse &browse)
{
    emit albumBrowseCompleted(browse);
}

void SonataPrivate::SpFactory::signalSearchCompleted(const SpSearch &search)
{
    emit searchCompleted(search);
}

/* Begin playlist callbacks */
static void SP_CALLCONV handlePlaylistMetadataUpdated(sp_playlist *, void *userdata)
{
    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    QEvent event((QEvent::Type) PlaylistMetadataUpdatedEvent);
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handlePlaylistStateChanged(sp_playlist *, void *userdata)
{
    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    QEvent event((QEvent::Type) PlaylistStateChangedEvent);
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handlePlaylistUpdateInProgress(sp_playlist *, bool done, void *userdata)
{
    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    QEvent::Type type = static_cast<QEvent::Type>(done ? PlaylistUpdateFinishedEvent : PlaylistUpdateStartedEvent);
    QEvent event(type);
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handleDescriptionChanged(sp_playlist *, const char *, void *userdata)
{
    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    QEvent::Type type = static_cast<QEvent::Type>(PlaylistDescriptionChangedEvent);
    QEvent event(type);
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handlePlaylistRenamed(sp_playlist *, void *userdata)
{
    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    QEvent::Type type = static_cast<QEvent::Type>(PlaylistRenamedEvent);
    QEvent event(type);
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handleImageChanged(sp_playlist *, const byte*, void *userdata)
{
    qDebug() << "Got image changed.";
    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    QEvent::Type type = static_cast<QEvent::Type>(PlaylistImageChangedEvent);
    QEvent event(type);
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handleTracksAdded(sp_playlist *, sp_track *const *tracks, int num_tracks, int position, void *userdata)
{

    /*  NOTE: It may be necessary to provide an SpPlaylist with the event for the watcher to verify that the event
     *        is correlated with the correct playlist, although considering watchers unregister playlist when changing playlists
     *        and sendEvent is used here, I don't think it should be a problem, but this depends on the nature of libspotify's internal
     *        implementation. */

    QList<SpTrack> trackList;
    trackList.reserve(num_tracks);

    SpHandle core = SpCore::handle();
    Q_ASSERT(!core.isNull());

    for (int i = 0; i < num_tracks; ++i)
    {
        SpTrack track = core->factory()->createTrack(*(tracks + i));
        trackList.append(track);
    }

    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    PlaylistModificationEvent event(PlaylistTracksAddedEvent);
    event.tracks = trackList;
    event.position = position;
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handleTracksRemoved(sp_playlist *, const int *tracks, int num_tracks, void *userdata)
{
    QVector<int> trackPositions(num_tracks);

    for (int i = 0; i < num_tracks; ++i)
        trackPositions.append(*(tracks + i));

    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    PlaylistModificationEvent event(PlaylistTracksRemovedEvent);
    event.trackPositions = trackPositions;
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handleTracksMoved(sp_playlist *, const int *tracks, int num_tracks, int new_position, void *userdata)
{
    QVector<int> trackPositions(num_tracks);

    for (int i = 0; i < num_tracks; ++i)
        trackPositions.append(*(tracks + i));

    SpPlaylistWatcher * watcher = static_cast<SpPlaylistWatcher *>(userdata);
    PlaylistModificationEvent event(PlaylistTracksMovedEvent);
    event.trackPositions = trackPositions;
    event.position = new_position;
    QCoreApplication::sendEvent(watcher, &event);
}

/* End playlist callbacks */

/* Begin playlist container callbacks */
static void SP_CALLCONV handlePlaylistContainerLoaded(sp_playlistcontainer *, void * userdata)
{
    SpPlaylistContainerWatcher * watcher = static_cast<SpPlaylistContainerWatcher *>(userdata);
    QEvent event((QEvent::Type) ContainerLoadedEvent);
    QCoreApplication::sendEvent(watcher, &event);

}

static void SP_CALLCONV handlePlaylistAdded(sp_playlistcontainer *, sp_playlist *, int position, void *userdata)
{
    SpPlaylistContainerWatcher * watcher = static_cast<SpPlaylistContainerWatcher *>(userdata);
    ContainerModificationEvent event(PlaylistAddedEvent);
    event.position = position;
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handlePlaylistRemoved(sp_playlistcontainer *, sp_playlist *, int position, void *userdata)
{
    SpPlaylistContainerWatcher * watcher = static_cast<SpPlaylistContainerWatcher *>(userdata);
    ContainerModificationEvent event(PlaylistRemovedEvent);
    event.position = position;
    QCoreApplication::sendEvent(watcher, &event);
}

static void SP_CALLCONV handlePlaylistMoved(sp_playlistcontainer *, sp_playlist *, int position, int new_position, void *userdata)
{
    SpPlaylistContainerWatcher * watcher = static_cast<SpPlaylistContainerWatcher *>(userdata);
    ContainerModificationEvent event(PlaylistMovedEvent);
    event.position = position;
    event.newPosition = new_position;
    QCoreApplication::sendEvent(watcher, &event);
}

/* End playlist container callbacks */

/* Begin image callbacks */
static void SP_CALLCONV handleImageLoaded(sp_image *, void *userdata)
{
    SpImageWatcher * watcher = static_cast<SpImageWatcher *>(userdata);
    QEvent event((QEvent::Type) ImageLoadedEvent);

    QCoreApplication::sendEvent(watcher, &event);
}

/* End image callbacks */

/* Begin artist browse callbacks */

static void SP_CALLCONV handleArtistBrowseComplete(sp_artistbrowse * result, void *)
{
    SpHandle core = SpCore::handle();
    if (core.isNull())
        return;

    SpArtistBrowse browse = core->factory()->createArtistBrowse(result);
    core->factory()->signalArtistBrowseCompleted(browse);
}

/* End artist browse callbacks */

/* Begin album browse callbacks */

static void SP_CALLCONV handleAlbumBrowseComplete(sp_albumbrowse * result, void *)
{
    SpHandle core = SpCore::handle();
    if (core.isNull())
        return;

    SpAlbumBrowse browse = core->factory()->createAlbumBrowse(result);
    core->factory()->signalAlbumBrowseCompleted(browse);
}

/* End album browse callbacks */

/* Begin search callbacks */

static void SP_CALLCONV handleSearchComplete(sp_search *result, void *)
{
    SpHandle core = SpCore::handle();
    if (core.isNull())
        return;

    SpSearch search = core->factory()->createSearch(result);
    core->factory()->signalSearchCompleted(search);
}
