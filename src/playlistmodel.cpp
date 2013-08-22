#include "playlistmodel.h"
#include <QVariant>
#include <Sonata/SpTrack>
#include <Sonata/SpArtist>
#include <Sonata/SpAlbum>
#include <QDebug>
#include <QPersistentModelIndex>
#include <QStringList>
#include <QUuid>

static bool areArtistsLoaded(const SpTrack &track);

struct PlaylistEntry {
    PlaylistEntry() : trackLoaded(false), artistsLoaded(false), albumLoaded(false) { }
    SpTrack track;
    SpArtistList artists;
    SpAlbum album;

    QPersistentModelIndex index;

    bool trackLoaded;
    bool artistsLoaded;
    bool albumLoaded;
};

class PlaylistModel::PlaylistModelPrivate
{
public:
    PlaylistModelPrivate() { }
    SpPlaylistWatcher * watcher;

    QVector<SpTrack> tracks;
    QVector<PlaylistEntry * > unloadedEntries;
};

PlaylistModel::PlaylistModel(QObject *parent) :
    QAbstractListModel(parent), d(new PlaylistModelPrivate)
{
    d->watcher = new SpPlaylistWatcher(this);
    connect(d->watcher, SIGNAL(metadataUpdated()),
            this, SLOT(updateMetadata()));
    connect(d->watcher, SIGNAL(stateChanged()),
            this, SLOT(updateMetadata()));
    connect(d->watcher, SIGNAL(tracksAdded(SpTrackList,int)),
            this, SLOT(addTracks(SpTrackList,int)));
    connect(d->watcher, SIGNAL(tracksRemoved(QVector<int>)),
            this, SLOT(removeTracks(QVector<int>)));
    connect(d->watcher, SIGNAL(tracksMoved(QVector<int>,int)),
            this, SLOT(moveTracks(QVector<int>,int)));
    connect(d->watcher, SIGNAL(nameChanged()),
            this, SIGNAL(nameChanged()));

    connect(this, SIGNAL(playlistChanged()),
            this, SIGNAL(nameChanged()));
}

PlaylistModel::~PlaylistModel()
{
    delete d;
}

SpPlaylist PlaylistModel::playlist() const
{
    return d->watcher->playlist();
}

void PlaylistModel::setPlaylist(const SpPlaylist &playlist)
{
    if (d->watcher->playlist() == playlist)
        return;

    beginResetModel();
    d->watcher->blockSignals(true);
    d->watcher->setPlaylist(playlist);
    d->watcher->blockSignals(false);

    qDeleteAll(d->unloadedEntries.begin(), d->unloadedEntries.end());
    d->unloadedEntries.clear();
    int count = d->watcher->trackCount();
    d->tracks.resize(count);
    for (int i = 0; i < count; ++i)
        d->tracks[i] = d->watcher->trackAt(i);
    endResetModel();

    // Loop through tracks again to create unloaded entries with correct model indices
    for (int i = 0; i < count; ++i)
    {
        SpTrack track = d->tracks.at(i);

        if (!track.isLoaded() || !track.album().isLoaded() || !areArtistsLoaded(track))
        {
            PlaylistEntry * entry = new PlaylistEntry;
            entry->track = track;
            entry->index = index(i);
            d->unloadedEntries.append(entry);
        }
    }

    emit playlistChanged();
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? d->tracks.count() : 0;
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    Q_ASSERT(row < d->tracks.count());

    const SpTrack &track = d->tracks.at(row);

    if (role == NameRole)
        return track.name();
    else if (role == TrackRole)
        return QVariant::fromValue(track);
    else if (role == ArtistsRole)
        return QVariant::fromValue(track.artists());
    else if (role == AlbumRole)
        return QVariant::fromValue(track.album());
    else if (role == ArtistNamesRole)
    {
        QStringList names;
        for (int i = 0; i < track.artistCount(); ++i)
            names << track.artistAt(i).name();
        return names;
    }
    else if (role == AlbumNameRole)
        return track.album().name();
    else if (role == DurationRole)
        return track.duration();
    else if (role == IndexRole)
        return row;
    else if (role == IsLoadedRole)
        return track.isLoaded();

    return QVariant();
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(NameRole, "name");
    roles.insert(TrackRole, "track");
    roles.insert(ArtistsRole, "artists");
    roles.insert(AlbumRole, "album");
    roles.insert(ArtistNamesRole, "artistNames");
    roles.insert(AlbumNameRole, "albumName");
    roles.insert(DurationRole, "duration");
    roles.insert(IndexRole, "index");
    roles.insert(IsLoadedRole, "isLoaded");
    return roles;
}

QString PlaylistModel::name() const
{
    return d->watcher->name();
}

void PlaylistModel::updateMetadata()
{
    // Note to self: try to optimize this function
    QVector<PlaylistEntry *>::Iterator i = d->unloadedEntries.begin();

    while (i != d->unloadedEntries.end())
    {
        PlaylistEntry * entry = *i;
        bool updated = false;

        if (!entry->trackLoaded && entry->track.isLoaded())
        {
            entry->trackLoaded = true;
            entry->artists = entry->track.artists();
            entry->album = entry->track.album();
            updated = true;
        }

        if (entry->trackLoaded && !entry->artistsLoaded)
        {
            bool artistsLoaded = true;
            foreach (SpArtist artist, entry->artists)
            {
                if (!artist.isLoaded())
                    artistsLoaded = false;
            }

            if (artistsLoaded)
            {
                entry->artistsLoaded = true;
                updated = true;
            }
        }

        if (entry->trackLoaded && !entry->albumLoaded)
        {
            if (entry->album.isLoaded())
            {
                updated = true;
                entry->albumLoaded = true;
            }
        }

        // It's likely that metadata updates in batches, try to optimize for this later on?
        if (updated)
        {
            QModelIndex modelIndex = entry->index;
            emit dataChanged(modelIndex, modelIndex);
        }

        if (entry->trackLoaded && entry->artistsLoaded && entry->albumLoaded)
        {
            delete entry;
            i = d->unloadedEntries.erase(i);
        }
        else
            ++i;
    }
}

void PlaylistModel::addTracks(const SpTrackList &tracks, int position)
{
    int count = tracks.count();
    beginInsertRows(QModelIndex(), position, position + count - 1);
    d->tracks.insert(position, count, SpTrack());

    for (int i = 0; i < count; ++i)
    {
        SpTrack track = tracks.at(i);
        d->tracks[position + i] = track;

        if (!track.isLoaded() || !track.album().isLoaded() || !areArtistsLoaded(track))
        {
            PlaylistEntry * entry = new PlaylistEntry;
            entry->track = track;
            entry->index = index(i);
            d->unloadedEntries.append(entry);
        }
    }

    endInsertRows();
}

void PlaylistModel::removeTracks(const QVector<int> positions)
{
    int count = positions.count();

    for (int i = 0; i < count; ++i)
    {
        int pos = positions.at(i);
        // Need to optimize this when positions are adjacent
        beginRemoveRows(QModelIndex(), pos, pos);
        d->tracks.remove(pos);
        endRemoveRows();
    }
}



void PlaylistModel::moveTracks(const QVector<int> /*positions*/, int /*position*/)
{
//    int count = positions.count();

    // This needs to be fixed. Atm moving tracks isn't supported

//    for (int i = 0; i < count; ++i)
//    {
//        int oldPos = positions.at(i);
//        int newPos = position + i;
//        int destination = newPos > oldPos ? newPos - 1 : newPos;
//        SpTrack source = d->tracks.at(oldPos);
//        // Need to optimize this when positions are adjacent
//        qDebug() << "Moving from " << oldPos << "to" << newPos;
//        beginMoveRows(QModelIndex(), oldPos, oldPos, QModelIndex(), newPos);

//        if (newPos > oldPos)
//        {
//            for (int i = oldPos; i < destination; ++i)
//            {
//                SpTrack track = d->tracks.at(i + 1);
//                d->tracks.replace(i, track);
//            }
//        }
//        else
//        {
//            for (int i = oldPos; i > destination; --i)
//            {
//                SpTrack track = d->tracks.at(i - 1);
//                d->tracks.replace(i, track);
//            }
//        }

//        d->tracks.replace(destination, source);

//        endMoveRows();
//    }
}

bool areArtistsLoaded(const SpTrack &track)
{
    int count = track.artistCount();
    for (int i = 0; i < count; ++i)
    {
        if (!track.artistAt(i).isLoaded())
            return false;
    }

    return true;
}
