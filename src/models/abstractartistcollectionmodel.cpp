#include "abstractartistcollectionmodel.h"

#include <QDebug>

namespace Sonetta {

namespace {

QHash<int, QByteArray> createRoles()
{
    QHash<int, QByteArray> r;
    r.insert(AbstractArtistCollectionModel::ArtistRole, "artist");
    r.insert(AbstractArtistCollectionModel::IsValidRole, "isValid");
    r.insert(AbstractArtistCollectionModel::IsLoadedRole, "isLoaded" );
    r.insert(AbstractArtistCollectionModel::NameRole, "name" );
    r.insert(AbstractArtistCollectionModel::SmallPortraitUriRole, "smallPortraitUri" );
    r.insert(AbstractArtistCollectionModel::NormalPortraitUriRole, "normalPortraitUri" );
    r.insert(AbstractArtistCollectionModel::LargePortraitUriRole, "largePortraitUri");
    return r;
}

const QHash<int, QByteArray> g_roleNames = createRoles();

}

AbstractArtistCollectionModel::AbstractArtistCollectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant AbstractArtistCollectionModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    Spotinetta::Artist artist = getArtistAt(row);
    Role r = (Role) role;

    switch (r)
    {
    case ArtistRole:
        return QVariant::fromValue(artist);
    case IsValidRole:
        return artist.isValid();
    case IsLoadedRole:
        return artist.isLoaded();
    case NameRole:
        return artist.name();
//    case SmallPortraitUriRole:
//        return SpLink::fromArtistPortrait(artist, Spotify::ImageSizeSmall).uri();
//    case NormalPortraitUriRole:
//        return SpLink::fromArtistPortrait(artist, Spotify::ImageSizeNormal).uri();
//    case LargePortraitUriRole:
//        return SpLink::fromArtistPortrait(artist, Spotify::ImageSizeLarge).uri();
    default:
        return QVariant();
    }
}

int AbstractArtistCollectionModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : getArtistCount();
}

QHash<int, QByteArray> AbstractArtistCollectionModel::roleNames() const
{
    return g_roleNames;
}

void AbstractArtistCollectionModel::updateData(int first, int last)
{
    QModelIndex begin = index(first);
    QModelIndex end = last == -1 ? index(first) : index(last);

    emit dataChanged(begin, end);
}

}
