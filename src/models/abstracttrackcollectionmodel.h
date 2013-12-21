#pragma once

#include <QAbstractListModel>
#include <QVector>
#include <QPointer>
#include <QPersistentModelIndex>
#include <Spotinetta/Spotinetta>

namespace Sonetta {

/*!
 * \brief The AbstractTrackCollectionModel class defines common roleNames and
 *  necessary model class implementations for models representing collections of tracks.
 */


class AbstractTrackCollectionModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum Role {
        TrackRole = Qt::UserRole,
        ArtistsRole,
        AlbumRole,
        IsLoadedRole,
        IsValidRole,
        NameRole,
        ArtistNamesRole,
        AlbumNameRole,
        DurationRole,
        PopularityRole,
        DiscRole,
        AlbumIndexRole,
        IsStarred,
        IsPlaceholder,
        IsLocal,
        IsAutoLinked,
        Availability,

        // Used in subclasses
        LastTrackCollectionModelRole
    };

    explicit AbstractTrackCollectionModel(const Spotinetta::Session * session, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;

public slots:
    void updateMetadata();

protected:
    virtual Spotinetta::Track getTrackAt(int index) const = 0;
    virtual int getTrackCount() const = 0;

private slots:
    void onRowsInserted(const QModelIndex & parent, int start, int end);
    void onModelReset();

private:
    void checkLoadStatus(int start, int end);

    void updateTrackMetadata();
    void updateAlbumMetadata();
    void updateArtistMetadata();
    void batchedDataUpdate(const QVector<QPersistentModelIndex> &objects, const QVector<int> &roles);
    Q_DISABLE_COPY(AbstractTrackCollectionModel)

    QVector< QPersistentModelIndex > m_pendingTracks;
    QVector< QPersistentModelIndex > m_pendingArtists;
    QVector< QPersistentModelIndex > m_pendingAlbums;

    QPointer<const Spotinetta::Session> m_session;
};

}
