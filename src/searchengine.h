#pragma once

#include <Spotinetta/Spotinetta>
#include <QObject>
#include <QStringList>
#include <QSharedPointer>

#include "models/tracklistmodel.h"
#include "models/albumlistmodel.h"
#include "models/artistlistmodel.h"

#include "utilities/pointers.h"

#include "settings.h"

namespace Sonetta {

class SearchEngine : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString query READ query NOTIFY queryChanged)
    Q_PROPERTY(QObject * tracks READ tracks CONSTANT)
    Q_PROPERTY(QObject * albums READ albums CONSTANT)
    Q_PROPERTY(QObject * artists READ artists CONSTANT)
    Q_PROPERTY(QStringList predictions READ predictions NOTIFY predictionsChanged)
    Q_PROPERTY(QStringList history READ history NOTIFY historyChanged)

public:
    explicit SearchEngine(ObjectSharedPointer<const Spotinetta::Session> session, ObjectSharedPointer<Settings> settings, QObject *parent = 0);

    QString query() const;

    QStringList predictions() const;
    QStringList history() const;

    QObject * tracks() const;
    QObject * albums() const;
    QObject * artists() const;

public slots:
    void go(const QString &query);
    void clear();
    void predict(const QString &partial);

signals:
    void predictionsChanged();
    void historyChanged();
    void queryChanged();

private slots:
    void onSearchLoaded();
    void onPredictionsLoaded();

    void fetchMoreTracks();
    void fetchMoreArtists();
    void fetchMoreAlbums();
    void fetchMorePlaylists();

private:
    void performQuery(int trackDelta, int albumDelta,
                      int artistDelta, int playlistDelta);

    void commitHistory();
    void loadHistory();
    void updateHistory(const QString & entry);

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

    QString     m_query;
    QStringList m_predictions;
    QStringList m_history;

    ObjectSharedPointer<const Spotinetta::Session>  m_session;
    ObjectSharedPointer<Settings>                   m_settings;

    ObjectScopedPointer<TrackListModel>             m_trackModel;
    ObjectScopedPointer<AlbumListModel>             m_albumModel;
    ObjectScopedPointer<ArtistListModel>            m_artistModel;
    ObjectScopedPointer<Spotinetta::SearchWatcher>  m_watcher;
    ObjectScopedPointer<Spotinetta::SearchWatcher>  m_predictionWatcher;
};

}
