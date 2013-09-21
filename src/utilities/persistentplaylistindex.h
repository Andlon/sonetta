#pragma once

#include <Spotinetta/Spotinetta>
#include <QPointer>

#include "qobjectscopedpointer.h"

namespace Sonetta {

class PersistentPlaylistIndex {
public:
    PersistentPlaylistIndex() : m_index(-1) { }
    PersistentPlaylistIndex(const Spotinetta::Session * session, const Spotinetta::Playlist & playlist, int index);
    ~PersistentPlaylistIndex();
    PersistentPlaylistIndex(const PersistentPlaylistIndex &other);
    PersistentPlaylistIndex & operator = (const PersistentPlaylistIndex &other);

    Spotinetta::Playlist playlist() const;
    int index() const;
    bool isValid() const;

private:
    void setup();
    void cleanup();

    QScopedPointer<Spotinetta::PlaylistWatcher, QObjectScopedPointerDeleter> m_watcher;
    QPointer<const Spotinetta::Session> m_session;
    int m_index;
};

inline bool operator == (const PersistentPlaylistIndex &a, const PersistentPlaylistIndex &b)
{
    return a.index() == b.index() && a.playlist() == b.playlist();
}
}

Q_DECLARE_METATYPE(Sonetta::PersistentPlaylistIndex)
