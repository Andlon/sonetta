#include "playlistmodel.h"

#include <QPersistentModelIndex>
#include <QtAlgorithms>

namespace sp = Spotinetta;

namespace Sonetta {

PlaylistModel::PlaylistModel(const Spotinetta::Session *session, QObject *parent)
    :   AbstractTrackCollectionModel(parent), m_isLoaded(false)
{
    m_watcher = new sp::PlaylistWatcher(session, this);

    connect(m_watcher, &sp::PlaylistWatcher::stateChanged,
            this, &PlaylistModel::onStateChanged);
    connect(m_watcher, &sp::PlaylistWatcher::metadataUpdated,
            this, &PlaylistModel::onMetadataUpdated);
    connect(m_watcher, &sp::PlaylistWatcher::tracksAdded,
            this, &PlaylistModel::onTracksAdded);
    connect(m_watcher, &sp::PlaylistWatcher::tracksMoved,
            this, &PlaylistModel::onTracksMoved);
    connect(m_watcher, &sp::PlaylistWatcher::tracksRemoved,
            this, &PlaylistModel::onTracksRemoved);
}

sp::Playlist PlaylistModel::playlist() const
{
    return m_watcher->watched();
}

void PlaylistModel::setPlaylist(const Spotinetta::Playlist &playlist)
{
    if (playlist != this->playlist())
    {
        beginResetModel();
        m_watcher->watch(playlist);
        m_isLoaded = false;
        m_tracks.clear();
        m_unloadedTracks.clear();
        m_unloadedAlbums.clear();
        m_unloadedArtists.clear();

        if (playlist.isLoaded())
        {
            onStateChanged();
        }
        endResetModel();
    }
}

Spotinetta::Track PlaylistModel::getTrackAt(int index) const
{
    Q_ASSERT(index >= 0 && index < m_tracks.count());
    return m_tracks.at(index);
}

int PlaylistModel::getTrackCount() const
{
    return m_tracks.count();
}

void PlaylistModel::onStateChanged()
{
    if (!m_isLoaded)
    {
        if (playlist().isLoaded())
        {
            m_isLoaded = true;
            loadTracks();
        }
    }
}

void PlaylistModel::loadTracks()
{
    // Assumes playlist is loaded
    sp::Playlist pl = playlist();
    m_tracks = pl.tracks().toVector();

    Q_ASSERT(pl.isLoaded());

    for (int i = 0; i < m_tracks.count(); ++i)
    {
        checkTrackLoadStatus(i);
    }
}

void PlaylistModel::checkTrackLoadStatus(int index)
{
    const sp::Track & track = m_tracks.at(index);
    if (!track.isLoaded())
    {
        // Add any unloaded tracks to a vector, which we'll
        // check upon metadata update
        QPersistentModelIndex modelIndex(this->index(index));
        m_unloadedTracks.append(qMakePair(track, modelIndex));
    }
    else
    {
        // Check loading status of album and artists
        checkAlbumLoadStatus(index);
        checkArtistsLoadStatus(index);
    }
}

void PlaylistModel::checkAlbumLoadStatus(int index)
{
    // Called immediately after a track has been loaded

    // Assumes track is loaded
    const sp::Track & track = m_tracks.at(index);
    const sp::Album & album = track.album();

    Q_ASSERT(track.isLoaded());

    if (!album.isLoaded())
    {
        m_unloadedAlbums.append(qMakePair(album, QPersistentModelIndex(this->index(index))));
    }
}

void PlaylistModel::checkArtistsLoadStatus(int index)
{
    // Called immediately after a track has been loaded

    // Assumes track is loaded
    const sp::Track & track = m_tracks.at(index);

    Q_ASSERT(track.isLoaded());

    for (int i = 0; i < track.artistCount(); ++i)
    {
        const sp::Artist artist = track.artistAt(index);

        if (!artist.isLoaded())
        {
            m_unloadedArtists.append(qMakePair(artist, QPersistentModelIndex(this->index(index))));
        }
    }
}

void PlaylistModel::onMetadataUpdated()
{
    // Update albums and artists first,
    // because any new unloaded entries added when iterating through
    // tracks will not have been loaded yet, and so will only
    // make iterating unloaded entries slower

    // Iterate through albums
    QVector<IndexedAlbum> keepAlbums;
    // Trade memory usage for speed. keepAlbums.size() <= m_unloadedAlbums.size()
    keepAlbums.reserve(m_unloadedAlbums.size());
    for (const IndexedAlbum & pair : m_unloadedAlbums)
    {
        const sp::Album & album = pair.first;
        const QPersistentModelIndex & modelIndex = pair.second;

        if (album.isLoaded())
        {
            updateAlbumData(modelIndex.row());
        }
        else
        {
            keepAlbums.append(pair);
        }
    }
    m_unloadedAlbums.swap(keepAlbums);

    // Iterate through artists
    QVector<IndexedArtist> keepArtists;
    keepArtists.reserve(m_unloadedArtists.size());
    for (const IndexedArtist & pair : m_unloadedArtists)
    {
        const sp::Artist & artist = pair.first;
        const QPersistentModelIndex & modelIndex = pair.second;

        if (artist.isLoaded())
        {
            updateArtistData(modelIndex.row());
        }
        else
        {
            keepArtists.append(pair);
        }
    }
    m_unloadedArtists.swap(keepArtists);

    // Iterate through tracks
    QVector<IndexedTrack> keepTracks;
    keepTracks.reserve(m_unloadedTracks.size());
    for (const IndexedTrack & pair : m_unloadedTracks)
    {
        const sp::Track & track = pair.first;
        const QPersistentModelIndex & modelIndex = pair.second;

        if (track.isLoaded())
        {
            int i = modelIndex.row();
            checkAlbumLoadStatus(i);
            checkArtistsLoadStatus(i);

            updateData(i);
        }
        else
        {
            keepTracks.append(pair);
        }
    }
    m_unloadedTracks.swap(keepTracks);

    // Release any extra reserved memory when all artists, albums and tracks have been loaded
    if (m_unloadedTracks.isEmpty()
            && m_unloadedAlbums.isEmpty()
            && m_unloadedArtists.isEmpty())
    {
        m_unloadedTracks.squeeze();
        m_unloadedAlbums.squeeze();
        m_unloadedArtists.squeeze();
    }
}

void PlaylistModel::onTracksAdded(const sp::TrackList &tracks, int position)
{
    Q_ASSERT(position >= 0 && position <= m_tracks.count());
    beginInsertRows(QModelIndex(), position, position + tracks.count() - 1);

    int newCount = m_tracks.count() + tracks.count();
    QVector<Spotinetta::Track> newTracks;
    newTracks.reserve(newCount);

    for (int i = 0; i < position; ++i)
        newTracks.append(m_tracks.at(i));

    for (int i = position; i < position + tracks.count(); ++i)
        newTracks.append(tracks.at(i));

    for (int i = position + tracks.count(); i < newCount; ++i)
        newTracks.append(m_tracks.at(i - position));

    endInsertRows();
}

void PlaylistModel::onTracksRemoved(const QVector<int> &positions)
{

}

void PlaylistModel::onTracksMoved(const QVector<int> &positions, int newPosition)
{

}


}
