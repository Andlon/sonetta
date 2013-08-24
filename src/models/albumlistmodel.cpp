#include "albumlistmodel.h"

#include <QMetaMethod>

namespace sp = Spotinetta;

namespace Sonetta {

AlbumListModel::AlbumListModel(QObject *parent)
    :   AbstractAlbumCollectionModel(parent)
{
}

void AlbumListModel::append(const sp::AlbumList &albums)
{
    if (albums.count() > 0)
    {
        int first = m_albums.count();
        int last = m_albums.count() + albums.count() - 1;

        beginInsertRows(QModelIndex(), first, last);
        m_albums.append(albums);
        endInsertRows();
    }
}

bool AlbumListModel::canFetchMore(const QModelIndex &parent) const
{
    // We can only fetch more if something's receiving notifications when we need more
    return !parent.isValid() && isSignalConnected(QMetaMethod::fromSignal(&AlbumListModel::needMore));
}

void AlbumListModel::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid())
        emit needMore();
}

void AlbumListModel::clear()
{
    beginResetModel();
    m_albums.clear();
    endResetModel();
}

sp::Album AlbumListModel::getAlbumAt(int index) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < m_albums.count());
    return m_albums[index];
}

int AlbumListModel::getAlbumCount() const
{
    return m_albums.count();
}

}
