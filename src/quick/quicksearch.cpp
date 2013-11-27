#include "quicksearch.h"

#include "../utilities/session.h"

#include <QDebug>

namespace sp = Spotinetta;

namespace Sonetta {

QuickSearch::QuickSearch(QObject *parent) :
    QObject(parent),
    m_trackOffset(0), m_albumOffset(0),
    m_playlistOffset(0), m_artistOffset(0)
{
    m_session = getCurrentSession();
    Q_ASSERT(m_session != nullptr);

    m_watcher = new sp::SearchWatcher(m_session, this);
    m_trackModel = new TrackListModel(this);
    m_albumModel = new AlbumListModel(this);
    m_artistModel = new ArtistListModel(this);

    connect(m_watcher, &sp::SearchWatcher::loaded,
            this, &QuickSearch::onSearchLoaded);

    // Set defaults
    m_albumDelta = 8;
    m_playlistDelta = 0;
    m_artistDelta = 8;
    m_trackDelta = 30;
}

void QuickSearch::performSearch(const QString &query, int trackDelta,
                                int albumDelta, int artistDelta, int playlistDelta)
{
    if (!m_session.isNull())
    {
        sp::Search search = m_session->createSearch(query, m_trackOffset, trackDelta,
                                          m_albumOffset, albumDelta,
                                          m_artistOffset, artistDelta,
                                          m_playlistOffset, playlistDelta,
                                          sp::Search::Type::Standard);
        m_watcher->watch(search);
    }
}

void QuickSearch::search(const QString &query)
{
    if (query != m_watcher->watched().query())
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
    return m_watcher->watched().query();
}

void QuickSearch::onSearchLoaded()
{
    sp::Search search = m_watcher->watched();

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
    if (m_watcher->watched().isLoaded())
        performSearch(m_watcher->watched().query(), m_trackDelta, 0, 0, 0);
}

void QuickSearch::fetchMoreAlbums()
{
    if (m_watcher->watched().isLoaded())
        performSearch(m_watcher->watched().query(), 0, m_albumDelta, 0, 0);
}

void QuickSearch::fetchMoreArtists()
{
    if (m_watcher->watched().isLoaded())
        performSearch(m_watcher->watched().query(), 0, 0, m_artistDelta, 0);
}

}
