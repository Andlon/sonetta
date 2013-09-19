#include "searchengine.h"

#include "utilities/fuzzystrings.h"
#include "utilities/predictioncollection.h"

#include <QPair>
#include <QVector>
#include <QtAlgorithms>

namespace sp = Spotinetta;

namespace Sonetta {

SearchEngine::SearchEngine(const Spotinetta::Session *session, QObject *parent)
    : QObject(parent), m_session(session)
{
    m_watcher = new sp::SearchWatcher(session, this);
    m_predictionWatcher = new sp::SearchWatcher(session, this);
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
    connect(m_predictionWatcher, &sp::SearchWatcher::loaded,
            this, &SearchEngine::onPredictionsLoaded);
}

QString SearchEngine::query() const
{
    return m_query;
}

QStringList SearchEngine::predictions() const
{
    return m_predictions;
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

        m_trackModel->clear();
        m_albumModel->clear();
        m_artistModel->clear();

        // Reconnect fetchMore if connections are broken (note UniqueConnection)
        connect(m_trackModel, &TrackListModel::needMore,
                this, &SearchEngine::fetchMoreTracks, Qt::UniqueConnection);
        connect(m_artistModel, &ArtistListModel::needMore,
                this, &SearchEngine::fetchMoreArtists, Qt::UniqueConnection);
        connect(m_albumModel, &AlbumListModel::needMore,
                this, &SearchEngine::fetchMoreAlbums, Qt::UniqueConnection);

        performQuery(m_trackDelta, m_albumDelta, m_artistDelta, m_playlistDelta);

        emit queryChanged();
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

void SearchEngine::predict(const QString &partial)
{
    if (partial.isEmpty())
    {
        m_predictions.clear();
        emit predictionsChanged();
        return;
    }
    if (!m_session.isNull())
    {
        sp::Search search = m_session->createSearch(partial, 0, 6, 0, 3, 0, 3, 0, 0, sp::Search::Type::Suggest);
        m_predictionWatcher->watch(search);

        if (search.isLoaded())
        {
            onPredictionsLoaded();
        }
    }
}

void SearchEngine::onPredictionsLoaded()
{
    sp::Search search = m_predictionWatcher->watched();

    QString query = search.query();

    // Mix track, artist and album predictions.
    sp::TrackList tracks = search.tracks();
    sp::ArtistList artists = search.artists();
    sp::AlbumList albums = search.albums();
    sp::PlaylistList playlists = search.playlists();

    PredictionCollection pred(query);

    for (const sp::Track & track : tracks)
    {
        pred.insert(track);
    }

    for (const sp::Artist & artist : artists)
    {
        pred.insert(artist);
    }

    for (const sp::Album & album : albums)
    {
        pred.insert(album);
    }

    for (const sp::Playlist & playlist : playlists)
    {
        pred.insert(playlist);
    }



    //    typedef QPair<QString, qreal> RankedMatch;
    //    QVector<RankedMatch> matches;

    //    auto rankString = [&] (const QString &name) -> RankedMatch
    //    {
    //        qreal rank = FuzzyStrings::levenshtein(query, name) / name.length();
    //        matches.append(RankedMatch(name, rank));
    //        return RankedMatch(name, rank);
    //    };

    //    for (const sp::Track &track : tracks)
    //    {
    //        matches.append(rankString(track.name()));
    //    }

    //    for (const sp::Artist & artist : artists)
    //    {
    //        matches.append(rankString(artist.name()));
    //    }

    //    for (const sp::Album & album : albums)
    //    {
    //        matches.append(rankString(album.name()));
    //    }

    //    qSort(matches.begin(), matches.end(), [] (const RankedMatch &a, const RankedMatch &b) {
    //        return a.second < b.second;
    //    });

    //    QStringList predictions;
    //    predictions.reserve(matches.size());
    //    for (const RankedMatch & match : matches)
    //    {
    //        predictions.append(match.first);
    //    }

    QStringList predictions = pred.predictions();
    // Limit to 8 predictions (the 8 best)
    if (predictions.size() > 8)
    {
        predictions.erase(predictions.begin() + 8, predictions.end());
    }

    m_predictions.swap(predictions);
    emit predictionsChanged();
}

}
