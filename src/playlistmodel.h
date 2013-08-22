#ifndef SPPLAYLISTMODEL_H
#define SPPLAYLISTMODEL_H

#include <Sonata/spplaylist.h>
#include <Sonata/spplaylistwatcher.h>

#include <QAbstractListModel>

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(SpPlaylist playlist READ playlist WRITE setPlaylist NOTIFY playlistChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
public:
    enum Role {
        TrackRole = Qt::UserRole,
        IsLoadedRole,
        ArtistsRole,
        AlbumRole,
        NameRole,
        ArtistNamesRole,
        AlbumNameRole,
        DurationRole,
        IndexRole
    };

    explicit PlaylistModel(QObject *parent = 0);
    ~PlaylistModel();
    
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QString name() const;

    QHash<int, QByteArray> roleNames() const;

    SpPlaylist playlist() const;
    void setPlaylist(const SpPlaylist &playlist);

signals:
    void playlistChanged();
    void nameChanged();

private slots:
    void updateMetadata();

    void addTracks(const SpTrackList &tracks, int position);
    void removeTracks(const QVector<int> positions);
    void moveTracks(const QVector<int> positions, int position);
    
private:
    Q_DISABLE_COPY(PlaylistModel)
    class PlaylistModelPrivate;
    PlaylistModelPrivate * d;
};

#endif // SPPLAYLISTMODEL_H
