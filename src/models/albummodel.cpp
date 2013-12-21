#include "albummodel.h"

#include "../application.h"

namespace sp = Spotinetta;

namespace Sonetta {

AlbumModel::AlbumModel(const Spotinetta::Session *session, QObject *parent)
    :   AbstractTrackCollectionModel(session, parent), m_session(session)
{
    m_watcher = new sp::AlbumBrowseWatcher(session, this);

    connect(m_watcher, &sp::AlbumBrowseWatcher::loaded,
            this, &AlbumModel::onLoaded);
    connect(m_watcher, &sp::AlbumBrowseWatcher::loaded,
            this, &AlbumModel::albumChanged);
    connect(this, &AlbumModel::albumChanged,
            this, &AlbumModel::albumMetadataChanged);
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

sp::Album AlbumModel::album() const
{
    return m_watcher->watched().album();
}

void AlbumModel::setAlbum(const sp::Album &album)
{
    if (m_session.isNull())
    {
        m_watcher->watch(sp::AlbumBrowse());
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

void AlbumModel::onLoaded()
{
    beginResetModel();
    m_tracks = m_watcher->watched().tracks();
    endResetModel();
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
