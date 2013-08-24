#include "quicksearch.h"


#include <QDebug>

namespace sp = Spotinetta;

namespace Sonetta {

QuickSearch::QuickSearch(QObject *parent) :
    QObject(parent), m_watcher(new sp::SearchWatcher),
    m_trackOffset(0), m_artistOffset(0),
    m_playlistOffset(0), m_albumOffset(0)
{
    m_trackModel = new TrackListModel(this);
    m_albumModel = new AlbumListModel(this);
    m_artistModel = new ArtistListModel(this);

    connect(m_watcher, &sp::SearchWatcher::loaded,
            this, &QuickSearch::handleSearchLoaded);

    // Set defaults
    m_albumDelta = 8;
    m_playlistDelta = 0;
    m_artistDelta = 8;
    m_trackDelta = 30;
}

void QuickSearch::performSearch(const QString &query, int trackDelta,
                                int albumDelta, int artistDelta, int playlistDelta)
{
    sp::Session * session = qobject_cast<sp::otifySession *>(m_spotifySession);

    if (session)
    {
        sp::Search search = session->search(query, m_trackOffset, trackDelta,
                                          m_albumOffset, albumDelta,
                                          m_artistOffset, artistDelta,
                                          m_playlistOffset, playlistDelta,
                                          sp::otify::StandardSearch);
        m_watcher->setSearch(search);
    }
}

void QuickSearch::search(const QString &query)
{
    if (query != m_watcher->search().query())
    {
        connect(m_trackModel, &TrackListModel::needMore,
                this, &QuickSearch::fetchMoreTracks, Qt::UniqueConnection);
        connect(m_albumModel, &AlbumListModel::needMore,
                this, &QuickSearch::fetchMoreAlbums, Qt::UniqueConnection);
        connect(m_artistModel, &ArtistListModel::needMore,
                this, &QuickSearch::fetchMoreArtists, Qt::UniqueConnection);

        m_trackOffset = 0;
        m_artistOffset = 0;
        m_playlistOffset = 0;
        m_albumOffset = 0;

        m_trackModel->clear();
        m_albumModel->clear();
        m_artistModel->clear();

        performSearch(query, m_trackDelta, m_albumDelta, m_artistDelta, m_playlistDelta);
        emit queryChanged();
    }
}

QString QuickSearch::query() const
{
    return m_watcher->search().query();
}

void QuickSearch::handleSearchLoaded()
{
    sp::Search search = m_watcher->search();

    m_albumOffset += search.albumCount();
    m_trackOffset += search.trackCount();
    m_artistOffset += search.artistCount();
    m_playlistOffset += search.playlistCount();

    if (search.trackCount() > 0)
    {
        m_trackModel->append(search.tracks());
    }
    else
    {
        disconnect(m_trackModel, &TrackListModel::needMore,
                   this, &QuickSearch::fetchMoreTracks);
    }

    if (search.albumCount() > 0)
    {
        m_albumModel->append(search.albums());
    }
    else
    {
        disconnect(m_albumModel, &AlbumListModel::needMore,
                   this, &QuickSearch::fetchMoreAlbums);
    }

    if (search.artistCount() > 0)
    {
        m_artistModel->append(search.artists());
    }
    else
    {
        disconnect(m_artistModel, &ArtistListModel::needMore,
                   this, &QuickSearch::fetchMoreArtists);
    }
}

void QuickSearch::fetchMoreTracks()
{
    if (m_watcher->search().isLoaded())
        performSearch(m_watcher->search().query(), m_trackDelta, 0, 0, 0);
}

void QuickSearch::fetchMoreAlbums()
{
    if (m_watcher->search().isLoaded())
        performSearch(m_watcher->search().query(), 0, m_albumDelta, 0, 0);
}

void QuickSearch::fetchMoreArtists()
{
    if (m_watcher->search().isLoaded())
        performSearch(m_watcher->search().query(), 0, 0, m_artistDelta, 0);
}

}
