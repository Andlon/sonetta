#ifndef SPPLAYLISTCONTAINERWATCHER_H
#define SPPLAYLISTCONTAINERWATCHER_H

#include <QObject>

#include "spplaylistcontainer.h"
#include "spplaylist.h"

class SpPlaylistContainerWatcher : public QObject {
    Q_OBJECT

    Q_PROPERTY(SpPlaylistContainer container READ container WRITE setContainer NOTIFY containerChanged)
    Q_PROPERTY(bool valid READ isValid NOTIFY containerChanged)
    Q_PROPERTY(bool loaded READ isLoaded NOTIFY containerLoaded)
    Q_PROPERTY(int playlistCount READ playlistCount NOTIFY dataChanged)
public:
    explicit SpPlaylistContainerWatcher(QObject * parent = 0);
    ~SpPlaylistContainerWatcher();

    SpPlaylistContainer container() const;
    void setContainer(const SpPlaylistContainer &container);

    SpPlaylistList playlists() const;
    Q_INVOKABLE SpPlaylist playlistAt(int index) const;

    int playlistCount() const;
    bool isValid() const;
    bool isLoaded() const;

    bool event(QEvent *);

signals:
    void containerChanged();
    void containerLoaded();
    void dataChanged();
    void playlistAdded(int position);
    void playlistRemoved(int position);
    void playlistMoved(int oldPosition, int newPosition);

private:
    Q_DISABLE_COPY(SpPlaylistContainerWatcher)
    SpPlaylistContainer m_container;
};

#endif // SPPLAYLISTCONTAINERWATCHER_H
