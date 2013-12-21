#include "tracklistmodel.h"

#include <QMetaMethod>

namespace sp = Spotinetta;

namespace Sonetta {

TrackListModel::TrackListModel(const Spotinetta::Session * session, QObject * parent)
    :   AbstractTrackCollectionModel(session, parent)
{
}

void TrackListModel::append(const sp::TrackList &tracks)
{
    if (tracks.count() > 0)
    {
        int first = m_tracks.count();
        int last = m_tracks.count() + tracks.count() - 1;

        beginInsertRows(QModelIndex(), first, last);
        m_tracks.append(tracks);
        endInsertRows();

        emit listChanged();
    }
}

bool TrackListModel::canFetchMore(const QModelIndex &parent) const
{
    // We can only fetch more if something's receiving notifications when we need more
    return !parent.isValid() && isSignalConnected(QMetaMethod::fromSignal(&TrackListModel::needMore));
}

void TrackListModel::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid())
        emit needMore();
}

void TrackListModel::clear()
{
    beginResetModel();
    m_tracks.clear();
    endResetModel();

    emit listChanged();
}

sp::TrackList TrackListModel::list() const
{
    return m_tracks;
}

sp::Track TrackListModel::getTrackAt(int index) const
{
    return m_tracks[index];
}

int TrackListModel::getTrackCount() const
{
    return m_tracks.count();
}

}
