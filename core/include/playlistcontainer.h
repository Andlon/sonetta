#pragma once

#include "playlist.h"
#include "user.h"
#include <QObject>
#include <QList>

namespace Sonetta {

struct PlaylistContainerData {
    QString name;
    QVariant id;
    User user;
    QList<Playlist> playlists;
};

class PlaylistContainer : public QObject {
public:
    QVariant id() const;
    QString service() const;

public slots:
    /**
     * @brief synchronize Synchronizes data across threads by signalling the current data to all
     * interested parties (slots connected to the synchronized signal). Since signals in Qt are sequentially
     * consistent across threads, subsequent signals such as playlistsAdded etc. will be received
     * in each thread in the correct order.
     */
    void synchronize();

signals:
    void synchronized(PlaylistContainerData data);

    void reset(PlaylistContainerData data);
    void playlistsAdded(QList<SharedPlaylist> playlists, int startIndex);
    void playlistsRemoved(int startIndex, int count);
    void renamed(QString newName);
    void userChanged(User newUser);

protected:
    PlaylistContainerData m_data;
};

// Move this class to internal sources when possible.
// It provides the core with an interface of mutators
// which are designed to be run in the thread of the container.
class MutablePlaylistContainer : public PlaylistContainer
{
public:
    void reset(const PlaylistContainerData & data);
    void remove(int startIndex, int count);
    void add(QList<SharedPlaylist> playlists, int count);
    void rename(const QString &name);
    void changeUser(const User &newUser);
};

}
