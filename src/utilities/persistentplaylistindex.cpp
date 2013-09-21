#include "persistentplaylistindex.h"

namespace sp = Spotinetta;

namespace Sonetta {

PersistentPlaylistIndex::PersistentPlaylistIndex(const Spotinetta::Session *session, const Spotinetta::Playlist &playlist, int index)
    :   m_index(index), m_session(session)
{
    setup();
    m_watcher->watch(playlist);
}

PersistentPlaylistIndex::PersistentPlaylistIndex(const PersistentPlaylistIndex &other)
    :   m_index(other.index()), m_session(other.m_session)
{
    setup();
    m_watcher->watch(other.playlist());
}

void PersistentPlaylistIndex::setup()
{
    m_watcher.reset(new sp::PlaylistWatcher(m_session));
    // Assumes playlist is loaded
    QObject::connect(m_watcher.data(), &sp::PlaylistWatcher::tracksAdded, [this] (const sp::TrackList &tracks, int position) {
        if (position <= m_index)
        {
            m_index += tracks.count();
        }
    });
    QObject::connect(m_watcher.data(), &sp::PlaylistWatcher::tracksMoved, [this] (const QVector<int> &positions, int newPosition) {
        int index = positions.indexOf(m_index);
        if (index >= 0)
        {
            m_index = newPosition + index;
        }
    });
    QObject::connect(m_watcher.data(), &sp::PlaylistWatcher::tracksRemoved, [this] (const QVector<int> &positions) {
        if (positions.contains(m_index))
        {
            m_index = -1;
        }
    });
}

void PersistentPlaylistIndex::cleanup()
{
    // Make sure we don't get any callbacks after [this] is destroyed
    m_watcher->watch(sp::Playlist());
}

PersistentPlaylistIndex::~PersistentPlaylistIndex()
{
    cleanup();
}

sp::Playlist PersistentPlaylistIndex::playlist() const
{
    return m_watcher->watched();
}

int PersistentPlaylistIndex::index() const
{
    return m_index;
}

bool PersistentPlaylistIndex::isValid() const
{
    return m_index >= 0 && m_index < playlist().trackCount();
}

PersistentPlaylistIndex & PersistentPlaylistIndex::operator = (const PersistentPlaylistIndex &other)
{
    cleanup();

    m_index = other.m_index;
    m_session = other.m_session;
    m_watcher->watch(other.playlist());

    return *this;
}

}
