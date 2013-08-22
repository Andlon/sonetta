#include "abstractartistcollectionmodel.h"

#include <Sonata/SpLink>
#include <QDebug>

static const QHash<int, QByteArray> g_roleNames = {
    { AbstractArtistCollectionModel::ArtistRole, "artist"},
    { AbstractArtistCollectionModel::IsValidRole, "isValid"},
    { AbstractArtistCollectionModel::IsLoadedRole, "isLoaded" },
    { AbstractArtistCollectionModel::NameRole, "name" },
    { AbstractArtistCollectionModel::SmallPortraitUriRole, "smallPortraitUri" },
    { AbstractArtistCollectionModel::NormalPortraitUriRole, "normalPortraitUri" },
    { AbstractArtistCollectionModel::LargePortraitUriRole, "largePortraitUri" }
};

AbstractArtistCollectionModel::AbstractArtistCollectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant AbstractArtistCollectionModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    SpArtist artist = getArtistAt(row);
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
    case SmallPortraitUriRole:
        return SpLink::fromArtistPortrait(artist, Spotify::ImageSizeSmall).uri();
    case NormalPortraitUriRole:
        return SpLink::fromArtistPortrait(artist, Spotify::ImageSizeNormal).uri();
    case LargePortraitUriRole:
        return SpLink::fromArtistPortrait(artist, Spotify::ImageSizeLarge).uri();
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
