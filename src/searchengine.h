#pragma once

#include <Spotinetta/Spotinetta>
#include <QObject>

#include "models/tracklistmodel.h"
#include "models/albumlistmodel.h"
#include "models/artistlistmodel.h"

namespace Sonetta {

class SearchEngine : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString query READ query NOTIFY queryChanged)
    Q_PROPERTY(QObject * tracks MEMBER m_trackModel CONSTANT)
    Q_PROPERTY(QObject * albums MEMBER m_albumModel CONSTANT)
    Q_PROPERTY(QObject * artists MEMBER m_artistModel CONSTANT)
public:
    explicit SearchEngine(const Spotinetta::Session * session, QObject *parent = 0);

    QString query() const;

public slots:
    void go(const QString &query);

signals:
    void queryChanged();

private slots:
    void onSearchLoaded();

    void fetchMoreTracks();
    void fetchMoreArtists();
    void fetchMoreAlbums();
    void fetchMorePlaylists();

private:
    void performQuery(int trackDelta, int albumDelta,
                      int artistDelta, int playlistDelta);

    int m_albumDelta;
    int m_trackDelta;
    int m_artistDelta;
    int m_playlistDelta;

    int m_trackOffset;
    int m_albumOffset;
    int m_playlistOffset;
    int m_artistOffset;

    // Used to keep track of whether or not
    // more data is fetchable
    int m_lastTrackDelta;
    int m_lastArtistDelta;
    int m_lastPlaylistDelta;
    int m_lastAlbumDelta;

    QString m_query;

    TrackListModel *    m_trackModel;
    AlbumListModel *    m_albumModel;
    ArtistListModel *   m_artistModel;

    QPointer<const Spotinetta::Session>     m_session;
    Spotinetta::SearchWatcher *             m_watcher;
};

}
