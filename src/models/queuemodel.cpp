#include "queuemodel.h"

#include <QDebug>

namespace sp = Spotinetta;

namespace Sonetta {

/*
 *  QueueContext
 */

QueueContext::QueueContext(const Spotinetta::Playlist &playlist)
    :   m_playlist(playlist), m_type(Type::Playlist)
{

}

QueueContext::QueueContext(const Spotinetta::TrackList &tracks)
    :   m_tracks(tracks), m_type(Type::TrackList)
{

}

QueueContext::QueueContext(const Spotinetta::AlbumBrowse &browse)
    :   m_browse(browse), m_type(Type::AlbumBrowse)
{

}

bool QueueContext::isValid() const
{
    switch (m_type)
    {
    case QueueContext::Type::AlbumBrowse:
        return m_browse.isValid();
    case QueueContext::Type::Playlist:
        return m_playlist.isValid();
    case QueueContext::Type::TrackList:
        return true;

    default:
        return false;
    }
}

bool QueueContext::isLoaded() const
{
    switch (m_type)
    {
    case QueueContext::Type::AlbumBrowse:
        return m_browse.isLoaded();
    case QueueContext::Type::Playlist:
        return m_playlist.isLoaded();
    case QueueContext::Type::TrackList:
        return true;

    default:
        return false;
    }
}

sp::Playlist QueueContext::playlist() const
{
    return m_playlist;
}

Spotinetta::AlbumBrowse QueueContext::albumBrowse() const
{
    return m_browse;
}

sp::TrackList QueueContext::tracks() const
{
    switch (m_type)
    {
    case QueueContext::Type::AlbumBrowse:
        return m_browse.tracks();
    case QueueContext::Type::Playlist:
        return m_playlist.tracks();
    case QueueContext::Type::TrackList:
        return m_tracks;

    default:
        return sp::TrackList();
    }
}

QueueContext::Type QueueContext::type() const
{
    return m_type;
}

/*
 *  QueueModel
 */

QueueModel::QueueModel(ObjectSharedPointer<const Spotinetta::Session> session, QObject *parent)
    :    AbstractTrackCollectionModel(session, parent), m_session(session),
      m_index(0), m_loaded(false),
      m_albumBrowseWatcher(new sp::AlbumBrowseWatcher(session.data())),
      m_playlistWatcher(new sp::PlaylistWatcher(session.data()))
{
    connect(m_albumBrowseWatcher.data(), &sp::AlbumBrowseWatcher::loaded,
            this, &QueueModel::onLoaded);
    connect(m_playlistWatcher.data(), &sp::PlaylistWatcher::stateChanged,
            this, &QueueModel::onLoaded);
    connect(m_playlistWatcher.data(), &sp::PlaylistWatcher::metadataUpdated,
            this, &QueueModel::updateMetadata);
    connect(session.data(), &sp::Session::metadataUpdated,
            this, &QueueModel::updateMetadata);
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
    m_index = index;
    m_loaded = false;

    int begin = m_explicit.count();
    if (m_implicit.count() > 0)
    {
        int end = qMax(0, begin + m_implicit.count() - 1);
        beginRemoveRows(QModelIndex(), begin, end);
        m_implicit.clear();
        endRemoveRows();
    }

    m_context = context;
    m_albumBrowseWatcher->watch(context.albumBrowse());
    m_playlistWatcher->watch(context.playlist());

    if (context.isLoaded())
    {
        onLoaded();
    }
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

void QueueModel::onLoaded()
{
    if (!m_loaded && m_context.isLoaded())
    {
        sp::TrackList tracks = m_context.tracks();

        int begin = m_explicit.count();
        if (tracks.count() > 0 && m_index + 1 < tracks.count())
        {
            sp::TrackList queueTracks = tracks.mid(m_index + 1);
            int end = qMax(0, begin + queueTracks.count() - 1);
            beginInsertRows(QModelIndex(), begin, end);
            m_implicit.append(queueTracks);
            endInsertRows();
        }

        m_loaded = true;
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

void QueueModel::updateContext(const Spotinetta::Album &album, int index)
{
    setContext(QueueContext(m_session->browse(album)), index);
}

}
