#include "searchengine.h"

#include "utilities/predictioncollection.h"

#include <QPair>
#include <QVector>
#include <QtAlgorithms>

namespace sp = Spotinetta;

namespace Sonetta {

namespace {
const int MAXHISTORY = 40;
}

SearchEngine::SearchEngine(ObjectSharedPointer<const Spotinetta::Session> session, ObjectSharedPointer<Settings> settings, QObject * parent)
    : QObject(parent), m_session(session), m_settings(settings),
      m_trackModel(new TrackListModel(session, this)),
      m_albumModel(new AlbumListModel(this)),
      m_artistModel(new ArtistListModel(this)),
      m_watcher(new sp::SearchWatcher(session.data(), this)),
      m_predictionWatcher(new sp::SearchWatcher(session.data(), this))
{
    Q_ASSERT(session.data() != nullptr);

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
    connect(m_watcher.data(), &sp::SearchWatcher::loaded,
            this, &SearchEngine::onSearchLoaded);
    connect(m_predictionWatcher.data(), &sp::SearchWatcher::loaded,
            this, &SearchEngine::onPredictionsLoaded);

    connect(m_session.data(), &sp::Session::metadataUpdated,
            m_albumModel.data(), &AlbumListModel::updateMetadata);
    connect(m_session.data(), &sp::Session::metadataUpdated,
            m_trackModel.data(), &TrackListModel::updateMetadata);

    loadHistory();
}

QString SearchEngine::query() const
{
    return m_query;
}

QStringList SearchEngine::predictions() const
{
    return m_predictions;
}

QStringList SearchEngine::history() const
{
    return m_history;
}

QObject *SearchEngine::tracks() const
{
    return m_trackModel.data();
}

QObject *SearchEngine::albums() const
{
    return m_albumModel.data();
}

QObject *SearchEngine::artists() const
{
    return m_artistModel.data();
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
        connect(m_trackModel.data(), &TrackListModel::needMore,
                this, &SearchEngine::fetchMoreTracks, Qt::UniqueConnection);
        connect(m_artistModel.data(), &ArtistListModel::needMore,
                this, &SearchEngine::fetchMoreArtists, Qt::UniqueConnection);
        connect(m_albumModel.data(), &AlbumListModel::needMore,
                this, &SearchEngine::fetchMoreAlbums, Qt::UniqueConnection);

        performQuery(m_trackDelta, m_albumDelta, m_artistDelta, m_playlistDelta);

        emit queryChanged();

        updateHistory(query);
        commitHistory();
    }
}

void SearchEngine::clear()
{
    m_trackModel->clear();
    m_albumModel->clear();
    m_artistModel->clear();
    m_query.clear();
    m_watcher->watch(sp::Search());

    emit queryChanged();
}

void SearchEngine::performQuery(int trackDelta, int albumDelta,
                                int artistDelta, int playlistDelta)
{
    m_lastTrackDelta = trackDelta;
    m_lastPlaylistDelta = playlistDelta;
    m_lastAlbumDelta = albumDelta;
    m_lastArtistDelta = artistDelta;

    sp::Search search = m_session->search(m_query, m_trackOffset, trackDelta,
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

void SearchEngine::commitHistory()
{
    m_settings->commitSearchHistory(m_history);
}

void SearchEngine::loadHistory()
{
    m_history = m_settings->loadSearchHistory();
    emit historyChanged();
}

void SearchEngine::updateHistory(const QString &entry)
{
    m_history.removeOne(entry);
    m_history.prepend(entry);

    if (m_history.size() > MAXHISTORY)
        m_history.removeLast();

    emit historyChanged();
}

void SearchEngine::onSearchLoaded()
{
    sp::Search search = m_watcher->watched();

    // Disconnect fetch if unable to acquire more results (tracks/albums/artists/playlists)
    if (m_lastTrackDelta > 0 && search.trackCount() < m_lastTrackDelta)
        disconnect(m_trackModel.data(), &TrackListModel::needMore,
                   this, &SearchEngine::fetchMoreTracks);

    if (m_lastAlbumDelta > 0 && search.albumCount() < m_lastAlbumDelta)
        disconnect(m_albumModel.data(), &AlbumListModel::needMore,
                   this, &SearchEngine::fetchMoreAlbums);

    if (m_lastArtistDelta > 0 && search.artistCount() < m_lastArtistDelta)
        disconnect(m_artistModel.data(), &ArtistListModel::needMore,
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
        sp::Search search = m_session->search(partial, 0, 6, 0, 3, 0, 3, 0, 0, sp::Search::Type::Suggest);
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

    // Mix track, artist and album predictions.
    sp::TrackList tracks = search.tracks();
    sp::ArtistList artists = search.artists();
    sp::AlbumList albums = search.albums();
    sp::PlaylistList playlists = search.playlists();

    PredictionCollection pred(search.query());

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

    QStringList predictions = pred.predictions();
    // Limit to 6 predictions (the 6 best)
    if (predictions.size() > 6)
    {
        predictions.erase(predictions.begin() + 6, predictions.end());
    }

    m_predictions.swap(predictions);
    emit predictionsChanged();
}

}
