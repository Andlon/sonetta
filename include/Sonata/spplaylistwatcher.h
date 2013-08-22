#ifndef SPPLAYLISTWATCHER_H
#define SPPLAYLISTWATCHER_H

#include <QObject>
#include <QQueue>

#include "spplaylist.h"
#include "spimage.h"

class PlaylistModel;

class SpPlaylistWatcher : public QObject {
    Q_OBJECT

    Q_PROPERTY(SpPlaylist playlist READ playlist WRITE setPlaylist NOTIFY playlistChanged)
    Q_PROPERTY(SpImage image READ image NOTIFY imageChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY playlistChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY stateChanged)
public:
    explicit SpPlaylistWatcher(QObject * parent = 0);
    ~SpPlaylistWatcher();

    SpPlaylist playlist() const;
    void setPlaylist(const SpPlaylist &playlist);

    SpTrackList tracks() const;
    int trackCount() const;
    SpTrack trackAt(int index) const;

    QString name() const;
    QString description() const;

    SpImage image() const;

    bool isValid() const;
    bool isLoaded() const;

    bool event(QEvent *);

signals:
    void playlistChanged();
    void stateChanged();
    void metadataUpdated();
    void nameChanged();
    void descriptionChanged();
    void imageChanged();

    void tracksAdded(const SpTrackList &tracks, int position);
    void tracksRemoved(const QVector<int> &positions);
    void tracksMoved(const QVector<int> &positions, int position);

private:
    Q_DISABLE_COPY(SpPlaylistWatcher)
    SpPlaylist m_playlist;
    bool m_isUpdating;
    QQueue<QEvent> m_eventQueue;
};

#endif // SPPLAYLISTWATCHER_H
