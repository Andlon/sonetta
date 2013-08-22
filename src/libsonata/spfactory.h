#ifndef SPOTIFYOBJECTFACTORY_H
#define SPOTIFYOBJECTFACTORY_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QWeakPointer>
#include <QEvent>

#include <Sonata/definitions.h>

#include <Sonata/splink.h>
#include <Sonata/sptrack.h>
#include <Sonata/spartist.h>
#include <Sonata/spalbum.h>
#include <Sonata/spplaylist.h>
#include <Sonata/spplaylistwatcher.h>
#include <Sonata/spplaylistcontainer.h>
#include <Sonata/spplaylistcontainerwatcher.h>
#include <Sonata/spimage.h>
#include <Sonata/spimagewatcher.h>
#include <Sonata/spartistbrowse.h>
#include <Sonata/spalbumbrowse.h>
#include <Sonata/spsearch.h>

#include <libspotify/api.h>

namespace SonataPrivate {

class PlaylistCount;
class PlaylistContainerCount;
class ImageCount;

enum PlaylistContainerEvent {
    ContainerLoadedEvent = QEvent::User + 6000,
    PlaylistAddedEvent,
    PlaylistRemovedEvent,
    PlaylistMovedEvent
};

enum PlaylistEvent {
    PlaylistMetadataUpdatedEvent = QEvent::User + 6000,
    PlaylistStateChangedEvent,
    PlaylistUpdateStartedEvent,
    PlaylistUpdateFinishedEvent,
    PlaylistRenamedEvent,
    PlaylistImageChangedEvent,
    PlaylistDescriptionChangedEvent,
    PlaylistTracksAddedEvent,
    PlaylistTracksRemovedEvent,
    PlaylistTracksMovedEvent
};

enum ImageEvent {
    ImageLoadedEvent = QEvent::User + 6000
};

class ContainerModificationEvent : public QEvent {
public:
    ContainerModificationEvent(PlaylistContainerEvent event)
        : QEvent((QEvent::Type) event), position(0), newPosition(0) { }

    int position;
    int newPosition;
};

class PlaylistModificationEvent : public QEvent {
public:
    PlaylistModificationEvent(PlaylistEvent event)
        :   QEvent((QEvent::Type) event), position(0) { }

    SpTrackList     tracks;
    int             position;
    QVector<int>    trackPositions;
};

typedef QWeakPointer<sp_session> SessionHandle;

class SpFactory : public QObject
{
    Q_OBJECT

public:
    explicit SpFactory(QObject *parent = 0);

    SpTrack createTrack(sp_track * handle) const;
    SpArtist createArtist(sp_artist * handle) const;
    SpAlbum createAlbum(sp_album * handle) const;
    SpPlaylist createPlaylist(sp_playlist * handle) const;
    SpPlaylistContainer createPlaylistContainer(sp_playlistcontainer * handle) const;
    SpImage createImage(sp_image * handle) const;
    SpArtistBrowse createArtistBrowse(sp_artistbrowse *) const;
    SpAlbumBrowse createAlbumBrowse(sp_albumbrowse *) const;
    SpSearch createSearch(sp_search *handle) const;

    SpImage createImageFromId(const byte* id) const;

    SpLink createLink(sp_link * link) const;
    SpTrack createTrackFromLink(const SpLink &link) const;
    SpArtist createArtistFromLink(const SpLink &link) const;

    sp_track * getTrackHandle(const SpTrack &track) const;
    sp_image * getImageHandle(const SpImage &image) const;

    void registerPlaylistWatcher(SpPlaylistWatcher * watcher);
    void unregisterPlaylistWatcher(SpPlaylistWatcher * watcher);

    void registerContainerWatcher(SpPlaylistContainerWatcher * watcher);
    void unregisterContainerWatcher(SpPlaylistContainerWatcher * watcher);

    void registerImageWatcher(SpImageWatcher * watcher);
    void unregisterImageWatcher(SpImageWatcher * watcher);

    SpArtistBrowse browse(const SpArtist &artist, Spotify::ArtistBrowseType type = Spotify::ArtistBrowseNoTracks) const;
    SpAlbumBrowse browse(const SpAlbum& album);
    SpSearch search(const QString &query, int trackOffset, int maxTracks,
                                int albumOffset, int maxAlbums,
                                int artistOffset, int maxArtists,
                                int playlistOffset, int maxPlaylists,
                                Spotify::SearchType type);

    void signalArtistBrowseCompleted(const SpArtistBrowse& browse);
    void signalAlbumBrowseCompleted(const SpAlbumBrowse &browse);
    void signalSearchCompleted(const SpSearch &search);

signals:
    void trackUpdated(SpTrack track);
    void artistUpdated(SpArtist artist);
    void albumUpdated(SpAlbum album);

    void artistBrowseCompleted(const SpArtistBrowse &browse);
    void albumBrowseCompleted(const SpAlbumBrowse &browse);
    void searchCompleted(const SpSearch &search);

public slots:
    void updateMeta();

private:
    mutable QVector<SpTrack> m_pendingTracks;
    mutable QVector<SpAlbum> m_pendingAlbums;
    mutable QVector<SpArtist> m_pendingArtists;

    QVector<PlaylistCount>          m_registeredPlaylists;
    QVector<PlaylistContainerCount> m_registeredContainers;
    QVector<ImageCount>             m_registeredImages;

    sp_playlist_callbacks m_playlistCallbacks;
    sp_playlistcontainer_callbacks m_playlistContainerCallbacks;
    
};

class PlaylistCount // Should rename this to better reflect usage
{
public:
    PlaylistCount() : handle(0) { }
    sp_playlist * handle;
    QVector<SpPlaylistWatcher *> watchers;
};

class PlaylistContainerCount // Should rename this to better reflect usage
{
public:
    PlaylistContainerCount() : handle(0) { }
    sp_playlistcontainer * handle;
    QVector<SpPlaylistContainerWatcher *> watchers;
};

class ImageCount // Should rename this to better reflect usage
{
public:
    ImageCount() : handle(0) { }
    sp_image * handle;
    QVector<SpImageWatcher *> watchers;
};

class SpLinkPrivate
{
public:
    SpLinkPrivate() : handle(0) { }
    sp_link * handle;
    SessionHandle session;
};

class SpTrackPrivate
{
public:
    SpTrackPrivate() : handle(0) { }
    sp_track * handle;
    SessionHandle session;
};

class SpArtistPrivate
{
public:
    SpArtistPrivate() : handle(0) { }
    sp_artist * handle;
    SessionHandle session;
};

class SpAlbumPrivate
{
public:
    SpAlbumPrivate() : handle(0) { }
    sp_album * handle;
    SessionHandle session;
};

class SpPlaylistPrivate
{
public:
    SpPlaylistPrivate() : handle(0) { }
    sp_playlist * handle;
    SessionHandle session;
};

class SpPlaylistContainerPrivate
{
public:
    SpPlaylistContainerPrivate() : handle(0) { }
    sp_playlistcontainer * handle;
    SessionHandle session;
};

class SpImagePrivate
{
public:
    SpImagePrivate() : handle(0) { }
    sp_image * handle;
    SessionHandle session;
};

class SpArtistBrowsePrivate
{
public:
    SpArtistBrowsePrivate() : handle(0) { }
    sp_artistbrowse * handle;
    SessionHandle session;
};

class SpAlbumBrowsePrivate
{
public:
    SpAlbumBrowsePrivate() : handle(0) { }
    sp_albumbrowse * handle;
    SessionHandle session;
};

class SpSearchPrivate
{
public:
    SpSearchPrivate() : handle(0) { }
    sp_search * handle;
    SessionHandle session;
};
}

#endif // SPOTIFYOBJECTFACTORY_H
