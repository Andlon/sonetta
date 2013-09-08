#pragma once

#include <Spotinetta/Spotinetta>
#include <QObject>

#include "../models/tracklistmodel.h"
#include "../models/albumlistmodel.h"
#include "../models/artistlistmodel.h"

namespace Sonetta {

class QuickSearch : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int albumDelta MEMBER m_albumDelta NOTIFY albumDeltaChanged)
    Q_PROPERTY(int trackDelta MEMBER m_trackDelta NOTIFY trackDeltaChanged)
    Q_PROPERTY(int artistDelta MEMBER m_artistDelta NOTIFY artistDeltaChanged)
    Q_PROPERTY(int playlistDelta MEMBER m_playlistDelta NOTIFY playlistDeltaChanged)

    Q_PROPERTY(QString query READ query NOTIFY queryChanged)

    Q_PROPERTY(QObject * tracks MEMBER m_trackModel CONSTANT)
    Q_PROPERTY(QObject * albums MEMBER m_albumModel CONSTANT)
    Q_PROPERTY(QObject * artists MEMBER m_artistModel CONSTANT)

public:
    explicit QuickSearch(QObject *parent = 0);

    QString query() const;

public slots:
    void search(const QString &query);

    void fetchMoreAlbums();
    void fetchMoreArtists();
//    void fetchMorePlaylists();
    void fetchMoreTracks();

signals:
    void trackDeltaChanged();
    void artistDeltaChanged();
    void albumDeltaChanged();
    void playlistDeltaChanged();
    void sessionChanged();

    void queryChanged();

private slots:
    void onSearchLoaded();

private:
    void performSearch(const QString &query, int trackDelta,
                        int albumDelta, int artistDelta, int playlistDelta);
    int m_albumDelta;
    int m_trackDelta;
    int m_artistDelta;
    int m_playlistDelta;

    int m_trackOffset;
    int m_albumOffset;
    int m_playlistOffset;
    int m_artistOffset;

    TrackListModel *    m_trackModel;
    AlbumListModel *    m_albumModel;
    ArtistListModel *   m_artistModel;

    QPointer<Spotinetta::Session>   m_session;
    Spotinetta::SearchWatcher *     m_watcher;
};

}
