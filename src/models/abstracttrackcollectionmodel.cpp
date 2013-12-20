#include "abstracttrackcollectionmodel.h"
#include <QStringList>
#include <QPair>

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

void rowSort(QVector<QPersistentModelIndex> & indices)
{
    qSort(indices.begin(), indices.end(), [] (const QPersistentModelIndex & A, const QPersistentModelIndex & B)
    {
        return A.row() < B.row();
    });
}

}

AbstractTrackCollectionModel::AbstractTrackCollectionModel(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(this, &AbstractTrackCollectionModel::rowsInserted,
            this, &AbstractTrackCollectionModel::onRowsInserted);
    connect(this, &AbstractTrackCollectionModel::modelReset,
            this, &AbstractTrackCollectionModel::onModelReset);
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
    case ArtistsRole:
        return QVariant::fromValue(track.artists());
    case AlbumRole:
        return QVariant::fromValue(track.album());
    case IsLoadedRole:
        return track.isLoaded();
    case IsValidRole:
        return track.isValid();
    case NameRole:
        return track.name();
    case ArtistNamesRole:
    {
        QStringList names;
        int count = track.artistCount();
        for (int i = 0; i < count; ++i)
            names << track.artistAt(i).name();
        return names;
    }
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

void AbstractTrackCollectionModel::updateMetadata()
{
    updateTrackMetadata();
    updateAlbumMetadata();
    updateArtistMetadata();
}

void AbstractTrackCollectionModel::updateTrackMetadata()
{
    decltype(m_pendingTracks) updatedTracks;
    decltype(m_pendingTracks) stillPendingTracks;
    stillPendingTracks.reserve(m_pendingTracks.count());

    for (const auto & modelIndex : m_pendingTracks)
    {
        sp::Track track = getTrackAt(modelIndex.row());
        if (modelIndex.isValid())
        {
            if (track.isLoaded())
            {
                updatedTracks.append(modelIndex);
                m_pendingArtists.append(modelIndex);
                m_pendingAlbums.append(modelIndex);
            }
            else
            {
                stillPendingTracks.append(modelIndex);
            }
        }
    }
    m_pendingTracks.swap(stillPendingTracks);

    rowSort(updatedTracks);
    batchedDataUpdate(updatedTracks, g_trackRoles);
}

void AbstractTrackCollectionModel::updateAlbumMetadata()
{
    decltype(m_pendingAlbums) updatedAlbums;
    decltype(m_pendingAlbums) stillPendingAlbums;
    stillPendingAlbums.reserve(m_pendingAlbums.count());
    for (const auto & modelIndex : m_pendingAlbums)
    {
        if (modelIndex.isValid())
        {
            sp::Track track = getTrackAt(modelIndex.row());
            if (track.album().isLoaded())
            {
                updatedAlbums.append(modelIndex);
            }
            else
            {
                stillPendingAlbums.append(modelIndex);
            }
        }
    }
    m_pendingAlbums.swap(stillPendingAlbums);

    rowSort(updatedAlbums);
    batchedDataUpdate(updatedAlbums, g_albumRoles);
}

void AbstractTrackCollectionModel::updateArtistMetadata()
{
    decltype(m_pendingArtists) updatedArtists;
    decltype(m_pendingArtists) stillPendingArtists;
    stillPendingArtists.reserve(m_pendingArtists.count());
    for (const auto & modelIndex : m_pendingArtists)
    {
        if (modelIndex.isValid())
        {
            sp::Track track = getTrackAt(modelIndex.row());
            bool anyArtistLoaded = false;
            bool anyArtistNotLoaded = false;
            for (const sp::Artist & artist : track.artists())
            {
                if (artist.isLoaded())
                {
                    anyArtistLoaded = true;
                }
                else
                {
                    anyArtistNotLoaded = true;
                }
            }

            if (anyArtistLoaded)
                updatedArtists.append(modelIndex);

            if (anyArtistNotLoaded)
                stillPendingArtists.append(modelIndex);
        }
    }

    m_pendingArtists.swap(stillPendingArtists);

    rowSort(updatedArtists);
    batchedDataUpdate(updatedArtists, g_artistRoles);
}

void AbstractTrackCollectionModel::batchedDataUpdate(const QVector<QPersistentModelIndex> &objects, const QVector<int> &roles)
{
    // NOTE: Assumes input is sorted

    for (int i = 0; i < objects.count(); ++i)
    {
        int j = i;
        while (j + 1 < objects.count() && (j + 1) - i == objects[j + 1].row() - objects[i].row())
            ++j;

        // Update interval [i, j]
        QModelIndex first = objects[i];
        QModelIndex last = objects[j];
        emit dataChanged(first, last, roles);

        i = j + 1;
    }
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

void AbstractTrackCollectionModel::onRowsInserted(const QModelIndex &, int start, int end)
{
    checkLoadStatus(start, end);
}

void AbstractTrackCollectionModel::onModelReset()
{
    if (getTrackCount() > 0)
    {
        checkLoadStatus(0, getTrackCount() - 1);
    }
}

void AbstractTrackCollectionModel::checkLoadStatus(int start, int end)
{
    if (start )
    for (int i = start; i <= end; ++i)
    {
        QPersistentModelIndex modelIndex = index(i);
        sp::Track track = getTrackAt(i);
        if (!track.isLoaded())
        {
            m_pendingTracks.append(modelIndex);
        }
        else
        {
            if (!track.album().isLoaded())
            {
                m_pendingAlbums.append(modelIndex);
            }

            for (const sp::Artist & artist : track.artists())
            {
                if (!artist.isLoaded())
                {
                    m_pendingArtists.append(modelIndex);
                    break;
                }
            }
        }
    }
}

}
