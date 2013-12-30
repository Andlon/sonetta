#include "abstractalbumcollectionmodel.h"

#include <map>

#include <QHash>

namespace sp = Spotinetta;

namespace Sonetta {

namespace {

QHash<int, QByteArray> createRoles() {
    QHash<int, QByteArray> r;
    r.insert(AbstractAlbumCollectionModel::AlbumRole, "album" );
    r.insert(AbstractAlbumCollectionModel::ArtistRole, "artist" );
    r.insert(AbstractAlbumCollectionModel::IsValidRole, "isValid" );
    r.insert(AbstractAlbumCollectionModel::IsLoadedRole, "isLoaded" );
    r.insert(AbstractAlbumCollectionModel::IsAvailableRole, "isAvailable" );
    r.insert(AbstractAlbumCollectionModel::NameRole, "name" );
    r.insert(AbstractAlbumCollectionModel::YearRole, "year" );
    r.insert(AbstractAlbumCollectionModel::TypeRole, "type" );
    r.insert(AbstractAlbumCollectionModel::SmallCoverUriRole, "smallCoverUri" );
    r.insert(AbstractAlbumCollectionModel::NormalCoverUriRole, "normalCoverUri" );
    r.insert(AbstractAlbumCollectionModel::LargeCoverUriRole, "largeCoverUri" );
    r.insert(AbstractAlbumCollectionModel::ArtistNameRole, "artistName");
    return r;
}

QHash<int, QByteArray> const g_roleNames = createRoles();

}

AbstractAlbumCollectionModel::AbstractAlbumCollectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant AbstractAlbumCollectionModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    sp::Album album = getAlbumAt(row);
    Role r = (Role) role;

    switch (r)
    {
    case AlbumRole:
        return QVariant::fromValue(album);
        break;
    case ArtistRole:
        return QVariant::fromValue(album.artist());
        break;
    case IsValidRole:
        return album.isValid();
        break;
    case IsLoadedRole:
        return album.isLoaded();
        break;
    case IsAvailableRole:
        return album.isAvailable();
        break;
    case NameRole:
        return album.name();
        break;
    case YearRole:
        return album.year();
        break;
    case TypeRole:
        return static_cast<int>(album.type());
        break;
    case SmallCoverUriRole:
        return sp::Link::fromAlbumCover(album, sp::ImageSize::Small).uri();
        break;
    case NormalCoverUriRole:
        return sp::Link::fromAlbumCover(album, sp::ImageSize::Normal).uri();
        break;
    case LargeCoverUriRole:
        return sp::Link::fromAlbumCover(album, sp::ImageSize::Large).uri();
        break;
    case ArtistNameRole:
        return album.artist().name();
        break;

    default:
        return QVariant();
    }
}

int AbstractAlbumCollectionModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : getAlbumCount();
}

QHash<int, QByteArray> AbstractAlbumCollectionModel::roleNames() const
{
    return g_roleNames;
}

void AbstractAlbumCollectionModel::updateMetadata()
{
    decltype(m_pending) stillPending;
    for (const auto & modelIndex : m_pending)
    {
        if (modelIndex.isValid())
        {
            sp::Album album = getAlbumAt(modelIndex.row());
            if (album.isLoaded())
            {
                emit dataChanged(modelIndex, modelIndex);
            }
            else
            {
                stillPending.append(modelIndex);
            }
        }
    }
    m_pending.swap(stillPending);
}

void AbstractAlbumCollectionModel::onRowsInserted(const QModelIndex &, int start, int end)
{
    checkLoadStatus(start, end);
}

void AbstractAlbumCollectionModel::onModelReset()
{
    if (getAlbumCount() > 0)
    {
        checkLoadStatus(0, getAlbumCount() - 1);
    }
}

void AbstractAlbumCollectionModel::checkLoadStatus(int start, int end)
{
    for (int i = start; i <= end; ++i)
    {
        sp::Album album = getAlbumAt(i);
        if (!album.isLoaded())
        {
            m_pending.append(index(i));
        }
    }
}

}
