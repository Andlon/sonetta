#pragma once

#include <QAbstractListModel>
#include <Spotinetta/Spotinetta>

#include <QVector>
#include <QSignalMapper>
#include <QSharedPointer>

#include "../utilities/pointers.h"

namespace Sonetta {

class PlaylistContainerModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(Spotinetta::PlaylistContainer playlistContainer READ playlistContainer WRITE setPlaylistContainer NOTIFY playlistContainerChanged)
public:
    enum Role {
        PlaylistRole = Qt::UserRole,
        IsLoadedRole,
        IsValidRole,
        NameRole,
        DescriptionRole,
        ImageUriRole
    };

    explicit PlaylistContainerModel(ObjectSharedPointer<const Spotinetta::Session> session, QObject *parent = 0);
    
    Spotinetta::PlaylistContainer playlistContainer() const;
    void setPlaylistContainer(const Spotinetta::PlaylistContainer &container);

    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent) const;

signals:
    void playlistContainerChanged();

private slots:
    void onLoaded();
    void onPlaylistAdded(int position);
    void onPlaylistRemoved(int position);
    void onPlaylistMoved(int oldPosition, int newPosition);

    void onPlaylistStateChanged(int position);
    void onPlaylistRenamed(int position);
    void onPlaylistDescriptionChanged(int position);
    void onPlaylistImageChanged(int position);

private:
    void updateWatcherIndices(int first, int last);
    void updateMappings(Spotinetta::PlaylistWatcher * watcher, int index);
    ObjectSharedPointer<Spotinetta::PlaylistWatcher> createWatcher(int index);

    QVector< ObjectSharedPointer<Spotinetta::PlaylistWatcher> > m_playlists;

    ObjectSharedPointer<const Spotinetta::Session>              m_session;
    ObjectScopedPointer<Spotinetta::PlaylistContainerWatcher>   m_watcher;

    ObjectScopedPointer<QSignalMapper> m_stateChangedMapper;
    ObjectScopedPointer<QSignalMapper> m_renamedMapper;
    ObjectScopedPointer<QSignalMapper> m_imageChangedMapper;
    ObjectScopedPointer<QSignalMapper> m_descriptionChangedMapper;

    void ignoreIndex(int index);
    int correctedIndex(int originalIndex) const;
    QVector<int> m_ignoredIndices;
};

}
