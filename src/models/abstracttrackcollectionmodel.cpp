#include "abstracttrackcollectionmodel.h"
#include <Sonata/SpAlbum>
#include <Sonata/SpArtist>
#include <QStringList>

class ModelRoleNames : public QHash<int, QByteArray> {
public:
    ModelRoleNames() {
        insert(AbstractTrackCollectionModel::TrackRole, "track");
        insert(AbstractTrackCollectionModel::ArtistsRole, "artists");
        insert(AbstractTrackCollectionModel::AlbumRole, "album");
        insert(AbstractTrackCollectionModel::IsLoadedRole, "isLoaded");
        insert(AbstractTrackCollectionModel::IsValidRole, "isValid");
        insert(AbstractTrackCollectionModel::NameRole, "name");
        insert(AbstractTrackCollectionModel::ArtistNamesRole, "artistNames");
        insert(AbstractTrackCollectionModel::AlbumNameRole, "albumName");
        insert(AbstractTrackCollectionModel::DurationRole, "duration");
        insert(AbstractTrackCollectionModel::PopularityRole, "popularity");
    }
};

class TrackDataRoles : public QVector<int>
{
public:
    TrackDataRoles() {
        append(AbstractTrackCollectionModel::TrackRole);
        append(AbstractTrackCollectionModel::IsLoadedRole);
        append(AbstractTrackCollectionModel::IsValidRole);
        append(AbstractTrackCollectionModel::NameRole);
        append(AbstractTrackCollectionModel::DurationRole);
        append(AbstractTrackCollectionModel::PopularityRole);
    }
};

class AlbumDataRoles : public QVector<int>
{
public:
    AlbumDataRoles() {
        append(AbstractTrackCollectionModel::AlbumRole);
        append(AbstractTrackCollectionModel::AlbumNameRole);
    }
};

class ArtistDataRoles : public QVector<int>
{
public:
    ArtistDataRoles() {
        append(AbstractTrackCollectionModel::ArtistsRole);
        append(AbstractTrackCollectionModel::ArtistNamesRole);
    }
};

static const ModelRoleNames     g_roleNames;
static const TrackDataRoles     g_trackRoles;
static const AlbumDataRoles     g_albumRoles;
static const ArtistDataRoles    g_artistRoles;


AbstractTrackCollectionModel::AbstractTrackCollectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QVariant AbstractTrackCollectionModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    SpTrack track = getTrackAt(row);
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
