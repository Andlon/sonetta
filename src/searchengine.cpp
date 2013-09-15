#include "searchengine.h"

namespace sp = Spotinetta;

namespace Sonetta {

SearchEngine::SearchEngine(const Spotinetta::Session *session, QObject *parent)
    : QObject(parent), m_session(session)
{
    m_watcher = new sp::SearchWatcher(session, this);
    m_trackModel = new TrackListModel(this);
    m_albumModel = new AlbumListModel(this);
    m_artistModel = new ArtistListModel(this);

    // Initialize offsets and deltas to reasonable values
    m_albumOffset
            = m_trackOffset
            = m_artistOffset
            = m_playlistOffset
            = 0;

    m_albumDelta = 10;
    m_trackDelta = 30;
    m_artistDelta = 10;
    m_playlistDelta = 10;

    // Set up connections
    connect(m_watcher, &sp::SearchWatcher::loaded,
            this, &SearchEngine::onSearchLoaded);
}

QString SearchEngine::query() const
{
    return m_query;
}

void SearchEngine::go(const QString &query)
{
    if (m_watcher->watched().query() != query && !m_session.isNull())
    {
        m_query = query;

        // Clean new query, reset offsets
        m_albumOffset = 0;
        m_trackOffset = 0;
        m_artistOffset = 0;
        m_playlistOffset = 0;

        // Reconnect fetchMore if connections are broken (note UniqueConnection)
        connect(m_trackModel, &TrackListModel::needMore,
                this, &SearchEngine::fetchMoreTracks, Qt::UniqueConnection);
        connect(m_artistModel, &ArtistListModel::needMore,
                this, &SearchEngine::fetchMoreArtists, Qt::UniqueConnection);
        connect(m_albumModel, &AlbumListModel::needMore,
                this, &SearchEngine::fetchMoreAlbums, Qt::UniqueConnection);

        performQuery(m_trackDelta, m_albumDelta, m_artistDelta, m_playlistDelta);
    }
}

void SearchEngine::performQuery(int trackDelta, int albumDelta,
                                int artistDelta, int playlistDelta)
{
    m_lastTrackDelta = trackDelta;
    m_lastPlaylistDelta = playlistDelta;
    m_lastAlbumDelta = albumDelta;
    m_lastArtistDelta = artistDelta;

    sp::Search search = m_session->createSearch(m_query, m_trackOffset, trackDelta,
                                                m_albumOffset, albumDelta,
                                                m_artistOffset, artistDelta,
                                                m_playlistOffset, playlistDelta, sp::Search::Type::Standard);

    m_watcher->watch(search);

    // Search may be cached, in that case we need to check
    // if it was loaded instantaneously (I think?)
    if (search.isLoaded())
    {
        onSearchLoaded();
    }
}

void SearchEngine::onSearchLoaded()
{
    sp::Search search = m_watcher->watched();

    // Disconnect fetch if unable to acquire more results (tracks/albums/artists/playlists)
    if (m_lastTrackDelta > 0 && search.trackCount() < m_lastTrackDelta)
        disconnect(m_trackModel, &TrackListModel::needMore,
                   this, &SearchEngine::fetchMoreTracks);

    if (m_lastAlbumDelta > 0 && search.albumCount() < m_lastAlbumDelta)
        disconnect(m_albumModel, &AlbumListModel::needMore,
                   this, &SearchEngine::fetchMoreAlbums);

    if (m_lastArtistDelta > 0 && search.artistCount() < m_lastArtistDelta)
        disconnect(m_artistModel, &ArtistListModel::needMore,
                   this, &SearchEngine::fetchMoreArtists);

    // Update offsets
    m_albumOffset += search.albumCount();
    m_artistOffset += search.artistCount();
    m_trackOffset += search.trackCount();
    m_playlistOffset += search.playlistCount();

    // Update models
    m_trackModel->append(search.tracks());
    m_albumModel->append(search.albums());
    m_artistModel->append(search.artists());
}

void SearchEngine::fetchMoreTracks()
{
    if (!m_session.isNull())
    performQuery(m_trackDelta, 0, 0, 0);
}

void SearchEngine::fetchMoreArtists()
{
    if (!m_session.isNull())
    performQuery(0, 0, m_artistDelta, 0);
}

void SearchEngine::fetchMorePlaylists()
{
    if (!m_session.isNull())
    performQuery(0, 0, 0, m_lastPlaylistDelta);
}

void SearchEngine::fetchMoreAlbums()
{
    if (!m_session.isNull())
    performQuery(0, m_albumDelta, 0, 0);
}

}
