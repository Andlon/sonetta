#pragma once

#include <QAbstractListModel>
#include <Spotinetta/Spotinetta>

#include <QVector>
#include <QSignalMapper>
#include <QSharedPointer>

namespace Sonetta {

class PlaylistContainerModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(Spotinetta::PlaylistContainer playlistContainer READ playlistContainer NOTIFY playlistContainerChanged)
public:
    enum Role {
        PlaylistRole = Qt::UserRole,
        IsLoadedRole,
        IsValidRole,
        NameRole,
        DescriptionRole,
        ImageUriRole
    };

    explicit PlaylistContainerModel(const Spotinetta::Session * session, QObject *parent = 0);
    
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
    QSharedPointer<Spotinetta::PlaylistWatcher> createWatcher(int index);

    Spotinetta::PlaylistContainerWatcher *  m_watcher;
    QPointer<const Spotinetta::Session>           m_session;

    QSignalMapper   * m_stateChangedMapper;
    QSignalMapper   * m_renamedMapper;
    QSignalMapper   * m_imageChangedMapper;
    QSignalMapper   * m_descriptionChangedMapper;

    QVector< QSharedPointer<Spotinetta::PlaylistWatcher> > m_playlists;
};

}
