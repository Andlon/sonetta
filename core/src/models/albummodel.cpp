#include "albummodel.h"

#include "../application.h"

namespace sp = Spotinetta;

namespace Sonetta {

AlbumModel::AlbumModel(ObjectSharedPointer<const Spotinetta::Session> session, QObject *parent)
    :   AbstractTrackCollectionModel(session, parent), m_session(session),
      m_watcher(new sp::AlbumBrowseWatcher(session.data())), m_duration(0)
{
    connect(m_watcher.data(), &sp::AlbumBrowseWatcher::loaded,
            this, &AlbumModel::onLoaded);
    connect(m_watcher.data(), &sp::AlbumBrowseWatcher::loaded,
            this, &AlbumModel::albumChanged);
    connect(this, &AlbumModel::albumChanged,
            this, &AlbumModel::albumMetadataChanged);
    connect(this, &AlbumModel::modelReset,
            this, &AlbumModel::tracksChanged);
}

QString AlbumModel::name() const
{
    return album().name();
}

QString AlbumModel::artistName() const
{
    return m_watcher->watched().artist().name();
}

bool AlbumModel::isAvailable() const
{
    return album().isAvailable();
}

int AlbumModel::year() const
{
    return album().year();
}

Spotinetta::Album::Type AlbumModel::type() const
{
    return album().type();
}

Spotinetta::Artist AlbumModel::artist() const
{
    return album().artist();
}

Spotinetta::TrackList AlbumModel::tracks() const
{
    return m_tracks;
}

sp::Album AlbumModel::album() const
{
    return m_watcher->watched().album();
}

void AlbumModel::setAlbum(const sp::Album &album)
{
    if (album == m_watcher->watched().album())
        return;

    if (m_session.isNull())
    {
        beginResetModel();
        m_watcher->watch(sp::AlbumBrowse());
        m_tracks.clear();
        endResetModel();
        emit albumChanged();
    }
    else if (album != this->album())
    {
        sp::AlbumBrowse browse = m_session->browse(album);
        beginResetModel();
        m_watcher->watch(browse);
        m_tracks = browse.tracks();
        endResetModel();
        emit albumChanged();
    }

    recalculateTotalDuration();
}

QString AlbumModel::smallCoverUri() const
{

    return sp::Link::fromAlbumCover(album(), sp::ImageSize::Small).uri();
}

QString AlbumModel::normalCoverUri() const
{
    return sp::Link::fromAlbumCover(album(), sp::ImageSize::Normal).uri();
}

QString AlbumModel::largeCoverUri() const
{
    return sp::Link::fromAlbumCover(album(), sp::ImageSize::Large).uri();
}

int AlbumModel::totalDuration() const
{
    return m_duration;
}

int AlbumModel::count() const
{
    return tracks().count();
}

void AlbumModel::onLoaded()
{
    beginResetModel();
    m_tracks = m_watcher->watched().tracks();
    endResetModel();
    recalculateTotalDuration();
}

void AlbumModel::recalculateTotalDuration()
{
    int duration = 0;
    for (const sp::Track & track : m_tracks) {
        duration += track.duration();
    }
    m_duration = duration;
    emit totalDurationChanged();
}

sp::Track AlbumModel::getTrackAt(int index) const
{
    return m_watcher->watched().trackAt(index);
}

int AlbumModel::getTrackCount() const
{
    return m_tracks.count();
}

}
