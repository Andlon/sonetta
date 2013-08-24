#include "abstracttrackcollectionmodel.h"
#include <QStringList>

namespace sp = Spotinetta;

namespace Sonetta {

namespace {

QHash<int, QByteArray> createRoles() {
    QHash<int, QByteArray> r;
    r.insert(AbstractTrackCollectionModel::TrackRole, "track");
    r.insert(AbstractTrackCollectionModel::ArtistsRole, "artists");
    r.insert(AbstractTrackCollectionModel::AlbumRole, "album");
    r.insert(AbstractTrackCollectionModel::IsLoadedRole, "isLoaded");
    r.insert(AbstractTrackCollectionModel::IsValidRole, "isValid");
    r.insert(AbstractTrackCollectionModel::NameRole, "name");
    r.insert(AbstractTrackCollectionModel::ArtistNamesRole, "artistNames");
    r.insert(AbstractTrackCollectionModel::AlbumNameRole, "albumName");
    r.insert(AbstractTrackCollectionModel::DurationRole, "duration");
    r.insert(AbstractTrackCollectionModel::PopularityRole, "popularity");
    return r;
}

QVector<int> createTrackRoleVector() {
    QVector<int> v;
    v.append(AbstractTrackCollectionModel::TrackRole);
    v.append(AbstractTrackCollectionModel::IsLoadedRole);
    v.append(AbstractTrackCollectionModel::IsValidRole);
    v.append(AbstractTrackCollectionModel::NameRole);
    v.append(AbstractTrackCollectionModel::DurationRole);
    v.append(AbstractTrackCollectionModel::PopularityRole);
    return v;
}

QVector<int> createAlbumRoleVector() {
    QVector<int> v;
    v.append(AbstractTrackCollectionModel::AlbumRole);
    v.append(AbstractTrackCollectionModel::AlbumNameRole);
    return v;
}

QVector<int> createArtistRoleVector() {
    QVector<int> v;
    v.append(AbstractTrackCollectionModel::ArtistsRole);
    v.append(AbstractTrackCollectionModel::ArtistNamesRole);
    return v;
}

const QHash<int, QByteArray>    g_roleNames = createRoles();
const QVector<int>              g_trackRoles = createTrackRoleVector();
const QVector<int>              g_albumRoles = createAlbumRoleVector();
const QVector<int>              g_artistRoles = createArtistRoleVector();

}

AbstractTrackCollectionModel::AbstractTrackCollectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant AbstractTrackCollectionModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    sp::Track track = getTrackAt(row);
    Role r = (Role) role;

    switch (r)
    {
    case TrackRole:
        return QVariant::fromValue(track);
        break;
    case ArtistsRole:
        return QVariant::fromValue(track.artists());
        break;
    case AlbumRole:
        return QVariant::fromValue(track.album());
        break;
    case IsLoadedRole:
        return track.isLoaded();
        break;
    case IsValidRole:
        return track.isValid();
        break;
    case NameRole:
        return track.name();
        break;
    case ArtistNamesRole:
    {
        QStringList names;
        int count = track.artistCount();
        for (int i = 0; i < count; ++i)
            names << track.artistAt(i).name();
        return names;
    }
        break;
    case AlbumNameRole:
        return track.album().name();
        break;
    case DurationRole:
        return track.duration();
        break;
    case PopularityRole:
        return track.popularity();
        break;

    default:
        break;
    }

    return QVariant();
}

int AbstractTrackCollectionModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : getTrackCount();
}

QHash<int, QByteArray> AbstractTrackCollectionModel::roleNames() const
{
    return g_roleNames;
}

void AbstractTrackCollectionModel::updateTrackData(int first, int last)
{
    QModelIndex begin = index(first);
    QModelIndex end = last == -1 ? index(first) : index(last);

    emit dataChanged(begin, end, g_trackRoles);
}

void AbstractTrackCollectionModel::updateAlbumData(int first, int last)
{
    QModelIndex begin = index(first);
    QModelIndex end = last == -1 ? index(first) : index(last);

    emit dataChanged(begin, end, g_albumRoles);
}

void AbstractTrackCollectionModel::updateArtistData(int first, int last)
{
    QModelIndex begin = index(first);
    QModelIndex end = last == -1 ? index(first) : index(last);

    emit dataChanged(begin, end, g_artistRoles);
}

void AbstractTrackCollectionModel::updateData(int first, int last)
{
    QModelIndex begin = index(first);
    QModelIndex end = last == -1 ? index(first) : index(last);

    emit dataChanged(begin, end);
}

}
