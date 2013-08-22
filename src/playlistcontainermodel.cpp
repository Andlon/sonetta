#include "playlistcontainermodel.h"

#include <Sonata/SpPlaylistWatcher>
#include <Sonata/SpPlaylistContainerWatcher>
#include <Sonata/SpLink>
#include <Sonata/SpImage>

#include <QVector>

#include <QDebug>

class PlaylistContainerModel::PlaylistContainerModelPrivate
{
public:
    PlaylistContainerModelPrivate() { }

    SpPlaylistContainerWatcher * watcher;
    QVector<SpPlaylistWatcher *> watchers;

    QSignalMapper * mapper;

    bool loaded;

};

PlaylistContainerModel::PlaylistContainerModel(QObject *parent) :
    QAbstractListModel(parent), d(new PlaylistContainerModelPrivate)
{
    d->mapper = new QSignalMapper(this);
    d->watcher = new SpPlaylistContainerWatcher(this);
    d->loaded = false;

    connect(d->mapper, SIGNAL(mapped(int)),
            this, SLOT(updatePlaylist(int)));
    connect(d->watcher, SIGNAL(containerLoaded()),
            this, SLOT(updatePlaylists()));
    connect(d->watcher, SIGNAL(playlistAdded(int)),
            this, SLOT(addPlaylist(int)));
    connect(d->watcher, SIGNAL(playlistRemoved(int)),
            this, SLOT(removePlaylist(int)));
    connect(d->watcher, SIGNAL(playlistMoved(int,int)),
            this, SLOT(movePlaylist(int,int)));
}

PlaylistContainerModel::~PlaylistContainerModel()
{
    delete d;
}

SpPlaylistContainer PlaylistContainerModel::container() const
{
    return d->watcher->container();
}

void PlaylistContainerModel::setContainer(const SpPlaylistContainer &container)
{
    if (container == d->watcher->container())
        return;

    beginResetModel();

    d->loaded = false;

    int newCount = container.playlistCount();
    int oldCount = d->watchers.count();
    int limit = qMin<int>(newCount, oldCount);

    for (int i = 0; i < limit; i++)
    {
        SpPlaylistWatcher * watcher = d->watchers.at(i);
        watcher->blockSignals(true);
        watcher->setPlaylist(container.playlistAt(i));
        watcher->blockSignals(false);
    }

    for (int i = limit; i < oldCount; ++i)
    {
        SpPlaylistWatcher * watcher = d->watchers.at(i);
        watcher->disconnect(this);
        watcher->disconnect(d->mapper);
        watcher->deleteLater();
    }

    d->watchers.resize(newCount);

    for (int i = limit; i < newCount; ++i)
    {
        SpPlaylistWatcher * watcher = new SpPlaylistWatcher(this);
        d->watchers[i] = watcher;
        watcher->setPlaylist(container.playlistAt(i));
        connect(watcher, SIGNAL(stateChanged()), d->mapper, SLOT(map()));
        connect(watcher, SIGNAL(nameChanged()), d->mapper, SLOT(map()));
        connect(watcher, SIGNAL(imageChanged()), d->mapper, SLOT(map()));
        d->mapper->setMapping(watcher, i);
    }

    d->watcher->setContainer(container);
    endResetModel();

    emit containerChanged();
}

QVariant PlaylistContainerModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    Q_ASSERT(row < d->watchers.count());

    SpPlaylistWatcher * watcher = d->watchers.at(row);

    if (watcher->isValid())
    {
        if (role == (int) NameRole)
            return watcher->name();
        else if (role == (int) PlaylistRole)
            return QVariant::fromValue(watcher->playlist());
        else if (role == (int) IsLoadedRole)
            return watcher->isLoaded();
        else if (role == (int) ImageUriRole)
            return SpLink(watcher->playlist().image()).uri();
    }

    return QVariant();
}

QHash<int, QByteArray> PlaylistContainerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(NameRole, "name");
    roles.insert(PlaylistRole, "playlist");
    roles.insert(IsLoadedRole, "isLoaded");
    roles.insert(ImageUriRole, "imageUri");
    return roles;
}

int PlaylistContainerModel::rowCount(const QModelIndex &parent) const
{
    return !parent.isValid() ? d->watchers.count() : 0;
}

void PlaylistContainerModel::updatePlaylist(int index)
{
    QModelIndex playlistIndex = this->index(index);
    emit dataChanged(playlistIndex, playlistIndex);
}

void PlaylistContainerModel::updatePlaylists()
{

}

void PlaylistContainerModel::addPlaylist(int position)
{
    Q_ASSERT(position <= d->watchers.count());
    beginInsertRows(QModelIndex(), position, position);
    SpPlaylistWatcher * watcher = new SpPlaylistWatcher(this);
    d->watchers.insert(position, watcher);
    connect(watcher, SIGNAL(stateChanged()), d->mapper, SLOT(map()));
    connect(watcher, SIGNAL(nameChanged()), d->mapper, SLOT(map()));
    d->mapper->setMapping(watcher, position);
    watcher->setPlaylist(d->watcher->playlistAt(position));
    endInsertRows();
}

void PlaylistContainerModel::removePlaylist(int position)
{
    Q_ASSERT(position < d->watchers.count());
    beginRemoveRows(QModelIndex(), position, position);
    SpPlaylistWatcher * watcher = d->watchers.at(position);
    watcher->disconnect(this);
    d->watchers.remove(position);
    watcher->deleteLater();
    endRemoveRows();
}

void PlaylistContainerModel::movePlaylist(int oldPos, int newPos)
{
    Q_ASSERT(oldPos < d->watchers.count());
    Q_ASSERT(newPos < d->watchers.count());
    Q_ASSERT(oldPos != newPos);

    if (beginMoveRows(QModelIndex(), oldPos, oldPos, QModelIndex(), newPos))
    {
        SpPlaylistWatcher * source = d->watchers.at(oldPos);
        int destination = newPos > oldPos ? newPos - 1 : newPos;

        // Can probably write this slightly neater (one loop perhaps)
        if (newPos > oldPos)
        {
            for (int i = oldPos; i < destination; ++i)
            {
                SpPlaylistWatcher * watcher = d->watchers.at(i + 1);
                d->watchers.replace(i, watcher);
                d->mapper->setMapping(watcher, i);
            }
        }
        else
        {
            for (int i = oldPos; i > destination; --i)
            {
                SpPlaylistWatcher * watcher = d->watchers.at(i - 1);
                d->watchers.replace(i, watcher);
                d->mapper->setMapping(watcher, i);
            }
        }

        d->watchers.replace(destination, source);
        d->mapper->setMapping(source, destination);

        endMoveRows();
    }
}
