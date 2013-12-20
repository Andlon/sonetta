#include "playlistmodel.h"

#include <QPersistentModelIndex>
#include <QtAlgorithms>
#include <QDebug>

namespace sp = Spotinetta;

namespace Sonetta {

PlaylistModel::PlaylistModel(const Spotinetta::Session *session, QObject *parent)
    :   AbstractTrackCollectionModel(parent)
{
    m_watcher = new sp::PlaylistWatcher(session, this);

    connect(m_watcher, &sp::PlaylistWatcher::stateChanged,
            this, &PlaylistModel::onStateChanged);
    connect(m_watcher, &sp::PlaylistWatcher::metadataUpdated,
            this, &PlaylistModel::updateMetadata);
    connect(m_watcher, &sp::PlaylistWatcher::tracksAdded,
            this, &PlaylistModel::onTracksAdded);
    connect(m_watcher, &sp::PlaylistWatcher::tracksMoved,
            this, &PlaylistModel::onTracksMoved);
    connect(m_watcher, &sp::PlaylistWatcher::tracksRemoved,
            this, &PlaylistModel::onTracksRemoved);
}

sp::Playlist PlaylistModel::playlist() const
{
    return m_watcher->watched();
}

void PlaylistModel::setPlaylist(const Spotinetta::Playlist &playlist)
{
    if (playlist != this->playlist())
    {
        beginResetModel();
        m_watcher->watch(playlist);
        m_tracks.clear();

        if (playlist.isLoaded())
        {
            m_tracks.reserve(playlist.trackCount());
            for (int i = 0; i < playlist.trackCount(); ++i)
            {
                m_tracks.append(playlist.trackAt(i));
            }
        }

        endResetModel();

        emit playlistChanged();
    }
}

Spotinetta::Track PlaylistModel::getTrackAt(int index) const
{
    Q_ASSERT(index >= 0 && index < m_tracks.count());
    return m_tracks.at(index);
}

int PlaylistModel::getTrackCount() const
{
    return m_tracks.count();
}

void PlaylistModel::onStateChanged()
{

}

void PlaylistModel::onTracksAdded(const sp::TrackList &tracks, int position)
{
    Q_ASSERT(position >= 0 && position <= m_tracks.count());
    beginInsertRows(QModelIndex(), position, position + tracks.count() - 1);

    int newCount = m_tracks.count() + tracks.count();
    QVector<Spotinetta::Track> newTracks;
    newTracks.reserve(newCount);

    for (int i = 0; i < position; ++i)
        newTracks.append(m_tracks.at(i));

    for (int i = position; i < position + tracks.count(); ++i)
        newTracks.append(tracks.at(i));

    for (int i = position + tracks.count(); i < newCount; ++i)
        newTracks.append(m_tracks.at(i - position));

    endInsertRows();
}

void PlaylistModel::onTracksRemoved(const QVector<int> &positions)
{
    qDebug() << "Tracks removed from playlist" << m_watcher->watched().name() << positions;
}

void PlaylistModel::onTracksMoved(const QVector<int> &positions, int newPosition)
{
    qDebug() << "Tracks moved in playlist" << m_watcher->watched().name() << positions << "to" << newPosition;
}


}
