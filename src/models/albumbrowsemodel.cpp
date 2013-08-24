#include "albumbrowsemodel.h"

#include "../application.h"

namespace sp = Spotinetta;

namespace Sonetta {

AlbumBrowseModel::AlbumBrowseModel(Spotinetta::Session *session, QObject *parent)
    :   AbstractTrackCollectionModel(parent)
{
    m_watcher = new sp::AlbumBrowseWatcher(session, this);

    connect(m_watcher, &sp::AlbumBrowseWatcher::loaded,
            this, &AlbumBrowseModel::onLoaded);
}

sp::AlbumBrowse AlbumBrowseModel::albumBrowse() const
{
    return m_watcher->watched();
}

void AlbumBrowseModel::setAlbumBrowse(const sp::AlbumBrowse &browse)
{
    if (browse != m_watcher->watched())
    {
        beginResetModel();
        m_watcher->watch(browse);
        m_tracks = browse.tracks();
        endResetModel();

        emit albumBrowseChanged();
    }
}

void AlbumBrowseModel::onLoaded()
{
    beginResetModel();
    m_tracks = albumBrowse().tracks();
    endResetModel();
}

sp::Track AlbumBrowseModel::getTrackAt(int index) const
{
    return albumBrowse().trackAt(index);
}

int AlbumBrowseModel::getTrackCount() const
{
    return m_tracks.count();
}

}
