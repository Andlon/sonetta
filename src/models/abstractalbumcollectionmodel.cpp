#include "abstractalbumcollectionmodel.h"
#include <Sonata/SpArtist>
#include <Sonata/SpLink>

class AlbumRoleNames : public QHash<int, QByteArray> {
public:
    AlbumRoleNames() {
        insert(AbstractAlbumCollectionModel::AlbumRole, "album");
        insert(AbstractAlbumCollectionModel::ArtistRole, "artist");
        insert(AbstractAlbumCollectionModel::IsValidRole, "isValid");
        insert(AbstractAlbumCollectionModel::IsLoadedRole, "isLoaded");
        insert(AbstractAlbumCollectionModel::IsAvailableRole, "isAvailable");
        insert(AbstractAlbumCollectionModel::NameRole, "name");
        insert(AbstractAlbumCollectionModel::YearRole, "year");
        insert(AbstractAlbumCollectionModel::TypeRole, "type");
        insert(AbstractAlbumCollectionModel::SmallCoverUriRole, "smallCoverUri");
        insert(AbstractAlbumCollectionModel::NormalCoverUriRole, "normalCoverUri");
        insert(AbstractAlbumCollectionModel::LargeCoverUriRole, "largeCoverUri");
        insert(AbstractAlbumCollectionModel::ArtistNameRole, "artistName");
    }
};

static const AlbumRoleNames g_roleNames;

AbstractAlbumCollectionModel::AbstractAlbumCollectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant AbstractAlbumCollectionModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    SpAlbum album = getAlbumAt(row);
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
        return album.type();
        break;
    case SmallCoverUriRole:
        return SpLink::fromAlbumCover(album, Spotify::ImageSizeSmall).uri(); // Temporary
        break;
    case NormalCoverUriRole:
        return SpLink::fromAlbumCover(album, Spotify::ImageSizeNormal).uri();
        break;
    case LargeCoverUriRole:
        return SpLink::fromAlbumCover(album, Spotify::ImageSizeLarge).uri();
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

void AbstractAlbumCollectionModel::updateData(int first, int last)
{
    QModelIndex begin = index(first);
    QModelIndex end = last == -1 ? index(first) : index(last);

    emit dataChanged(begin, end);
}
