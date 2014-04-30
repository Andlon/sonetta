#include "abstractartistcollectionmodel.h"

#include <QDebug>

namespace sp = Spotinetta;

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
    connect(this, &AbstractArtistCollectionModel::rowsInserted,
            this, &AbstractArtistCollectionModel::onRowsInserted);
    connect(this, &AbstractArtistCollectionModel::modelReset,
            this, &AbstractArtistCollectionModel::onModelReset);
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
    case SmallPortraitUriRole:
        return sp::Link::fromArtistPortrait(artist, sp::ImageSize::Small).uri();
    case NormalPortraitUriRole:
        return sp::Link::fromArtistPortrait(artist, sp::ImageSize::Normal).uri();
    case LargePortraitUriRole:
        return sp::Link::fromArtistPortrait(artist, sp::ImageSize::Large).uri();
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

void AbstractArtistCollectionModel::updateMetadata()
{
    decltype(m_pending) stillPending;
    for (const auto & modelIndex : m_pending)
    {
        if (modelIndex.isValid())
        {
            sp::Artist album = getArtistAt(modelIndex.row());
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

void AbstractArtistCollectionModel::onRowsInserted(const QModelIndex &, int start, int end)
{
    checkLoadStatus(start, end);
}

void AbstractArtistCollectionModel::onModelReset()
{
    if (getArtistCount() > 0)
    {
        checkLoadStatus(0, getArtistCount() - 1);
    }
}

void AbstractArtistCollectionModel::checkLoadStatus(int start, int end)
{
    for (int i = start; i <= end; ++i)
    {
        sp::Artist artist = getArtistAt(i);
        if (!artist.isLoaded())
        {
            m_pending.append(index(i));
        }
    }
}

}
