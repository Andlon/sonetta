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
//    case SmallCoverUriRole:
//        return SpLink::fromAlbumCover(album, Spotify::ImageSizeSmall).uri(); // Temporary
//        break;
//    case NormalCoverUriRole:
//        return SpLink::fromAlbumCover(album, Spotify::ImageSizeNormal).uri();
//        break;
//    case LargeCoverUriRole:
//        return SpLink::fromAlbumCover(album, Spotify::ImageSizeLarge).uri();
//        break;
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

void AbstractAlbumCollectionModel::updateData(int first, int last)
{
    QModelIndex begin = index(first);
    QModelIndex end = last == -1 ? index(first) : index(last);

    emit dataChanged(begin, end);
}

}
