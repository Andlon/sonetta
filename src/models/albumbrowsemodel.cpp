#include "albumbrowsemodel.h"

#include <QDebug>

AlbumBrowseModel::AlbumBrowseModel(QObject *parent)
    :   AbstractTrackCollectionModel(parent), m_watcher(new SpAlbumBrowseWatcher)
{
    connect(m_watcher, SIGNAL(albumBrowseChanged()),
            this, SIGNAL(albumBrowseChanged()));
    connect(m_watcher, SIGNAL(loaded()),
            this, SLOT(handleLoad()));
}

SpAlbumBrowse AlbumBrowseModel::albumBrowse() const
{
    qDebug() << "Browse validity: " << m_watcher->albumBrowse().isValid();
    return m_watcher->albumBrowse();
}

void AlbumBrowseModel::setAlbumBrowse(const SpAlbumBrowse &browse)
{
    beginResetModel();
    m_watcher->setAlbumBrowse(browse);
    m_tracks = browse.tracks();
    endResetModel();

    // Should not be necessary
    emit albumBrowseChanged();
}

void AlbumBrowseModel::handleLoad()
{
    beginResetModel();
    m_tracks = m_watcher->albumBrowse().tracks();
    endResetModel();
}

SpTrack AlbumBrowseModel::getTrackAt(int index) const
{
    return m_watcher->albumBrowse().trackAt(index);
}

int AlbumBrowseModel::getTrackCount() const
{
    return m_tracks.count();
}
