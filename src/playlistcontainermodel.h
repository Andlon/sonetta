#ifndef PlaylistContainerModel_H
#define PlaylistContainerModel_H

#include <Sonata/spplaylistcontainer.h>

#include <QAbstractListModel>
#include <QSignalMapper>

class PlaylistContainerModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(SpPlaylistContainer container READ container WRITE setContainer NOTIFY containerChanged)

public:
    enum Role {
        PlaylistRole = Qt::UserRole,
        NameRole,
        IsLoadedRole,
        ImageUriRole
    };

    explicit PlaylistContainerModel(QObject *parent = 0);
    ~PlaylistContainerModel();

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;

    QHash<int, QByteArray> roleNames() const;
    
    SpPlaylistContainer container() const;
    void setContainer(const SpPlaylistContainer &container);

signals:
    void containerChanged();

private slots:
    void updatePlaylist(int index);
    void updatePlaylists();

    void addPlaylist(int position);
    void removePlaylist(int position);
    void movePlaylist(int oldPos, int newPos);

private:
    Q_DISABLE_COPY(PlaylistContainerModel)
    class PlaylistContainerModelPrivate;
    PlaylistContainerModelPrivate * d;
};

#endif // PlaylistContainerModel_H
