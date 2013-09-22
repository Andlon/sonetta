#include "queuemodel.h"

namespace sp = Spotinetta;

namespace Sonetta {

/*
 *  QueueContext
 */

QueueContext::QueueContext(const Spotinetta::Playlist &playlist)
    :   m_playlist(playlist)
{

}

QueueContext::QueueContext(const Spotinetta::TrackList &tracks)
    :   m_tracks(tracks)
{

}

bool QueueContext::isValid() const
{
    return m_playlist.isValid() || !m_tracks.isEmpty();
}

sp::Playlist QueueContext::playlist() const
{
    return m_playlist;
}

sp::TrackList QueueContext::tracks() const
{
    return m_tracks;
}

QueueContext::Type QueueContext::type() const
{
    return m_type;
}

/*
 *  QueueModel
 */

QueueModel::QueueModel(QObject *parent) :
    AbstractTrackCollectionModel(parent)
{
}

QHash<int, QByteArray> QueueModel::roleNames() const
{
    QHash<int, QByteArray> roles = AbstractTrackCollectionModel::roleNames();
    roles.insert(Explicit, "isExplicit");
    return roles;
}

QVariant QueueModel::data(const QModelIndex &index, int role) const
{
    if (role == Explicit)
    {
        return (index.row() < m_explicit.count());
    }

    return AbstractTrackCollectionModel::data(index, role);
}

bool QueueModel::isEmpty() const
{
    return m_explicit.isEmpty() && m_implicit.isEmpty();
}

QueueContext QueueModel::context() const
{
    return m_context;
}

void QueueModel::setContext(const QueueContext &context, int index)
{
    sp::TrackList tracks;
    tracks.append(context.tracks());
    tracks.append(context.playlist().tracks());

    int begin = m_explicit.count();

    if (m_implicit.count() > 0)
    {
        int end = qMax(0, begin + m_implicit.count() - 1);
        beginRemoveRows(QModelIndex(), begin, end);
        m_implicit.clear();
        endRemoveRows();
    }

    if (tracks.count() > 0 && index + 1 < tracks.count())
    {
        sp::TrackList queueTracks = tracks.mid(index + 1);
        int end = qMax(0, begin + queueTracks.count() - 1);
        beginInsertRows(QModelIndex(), begin, end);
        m_implicit.append(queueTracks);
        endInsertRows();
    }

    m_context = context;
}

int QueueModel::getTrackCount() const
{
    return m_explicit.count() + m_implicit.count();
}

sp::Track QueueModel::getTrackAt(int index) const
{
    int explicitCount = m_explicit.count();
    if (index < explicitCount)
    {
        return m_explicit[index];
    }
    else
    {
        return m_implicit[index - explicitCount];
    }
}

void QueueModel::enqueue(const Spotinetta::Track &track)
{
    int begin = m_explicit.count();
    beginInsertRows(QModelIndex(), begin, begin);
    m_explicit.enqueue(track);
    endInsertRows();
}

// Removes the element in index from the list. If it's an implicit item,
// remove every implicit item preceding it (mimics the action taken by
// the desktop spotify client when you play an implicitly added track
// from the actual play queue window)
void QueueModel::pop(int index)
{
    if (index < 0 || index >= getTrackCount())
        return;

    if (index < m_explicit.count())
    {
        // Item is explicit, simply remove
        beginRemoveRows(QModelIndex(), index, index);
        m_explicit.removeAt(index);
        endRemoveRows();
    }
    else
    {
        // Item is implicit, remove every implicit item including index
        int begin = m_explicit.count();
        beginRemoveRows(QModelIndex(), begin, index);
        m_implicit.erase(m_implicit.begin(), m_implicit.begin() + (index - begin + 1));
        endRemoveRows();
    }
}

sp::Track QueueModel::dequeue()
{
    if (m_explicit.isEmpty() && m_implicit.isEmpty())
        return sp::Track();

    sp::Track track;

    beginRemoveRows(QModelIndex(), 0, 0);
    if (!m_explicit.isEmpty())
    {
        track = m_explicit.dequeue();
    }
    else if (!m_implicit.isEmpty())
    {
        track = m_implicit.dequeue();
    }
    endRemoveRows();

    return track;
}

void QueueModel::clearContext()
{
    setContext(QueueContext(), 0);
}

void QueueModel::updateContext(const Spotinetta::Playlist &playlist, int index)
{
    setContext(QueueContext(playlist), index);
}

void QueueModel::updateContext(const Spotinetta::TrackList &tracks, int index)
{
    setContext(QueueContext(tracks), index);
}

}
