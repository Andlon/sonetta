#pragma once

#include "abstracttrackcollectionmodel.h"
#include <QPair>
#include <QVector>

class QPersistentModelIndex;

namespace Sonetta {


class PlaylistModel : public AbstractTrackCollectionModel
{
    Q_OBJECT

    Q_PROPERTY(Spotinetta::Playlist playlist READ playlist WRITE setPlaylist NOTIFY playlistChanged)

public:
    explicit PlaylistModel(const Spotinetta::Session * session, QObject *parent = 0);

    Spotinetta::Playlist playlist() const;
    void setPlaylist(const Spotinetta::Playlist &playlist);

signals:
    void playlistChanged();

protected:
    Spotinetta::Track getTrackAt(int index) const;
    int getTrackCount() const;

private slots:
    void onMetadataUpdated();
    void onStateChanged();
    void onTracksAdded(const Spotinetta::TrackList &tracks, int position);
    void onTracksRemoved(const QVector<int> &positions);
    void onTracksMoved(const QVector<int> &positions, int newPosition);

private:
    Q_DISABLE_COPY(PlaylistModel)
    typedef QPair<Spotinetta::Track, QPersistentModelIndex>     IndexedTrack;
    typedef QPair<Spotinetta::Artist, QPersistentModelIndex>    IndexedArtist;
    typedef QPair<Spotinetta::Album, QPersistentModelIndex>     IndexedAlbum;

    void loadTracks();
    void checkTrackLoadStatus(int index);
    void checkArtistsLoadStatus(int index);
    void checkAlbumLoadStatus(int index);

    Spotinetta::PlaylistWatcher *   m_watcher;
    QVector<Spotinetta::Track>      m_tracks;
    QVector<IndexedTrack>           m_unloadedTracks;
    QVector<IndexedArtist>          m_unloadedArtists;
    QVector<IndexedAlbum>           m_unloadedAlbums;
    bool                            m_isLoaded;
};

}
