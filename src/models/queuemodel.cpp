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
