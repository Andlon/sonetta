#pragma once

#include <QAbstractListModel>

#include <Spotinetta/Spotinetta>

namespace Sonetta {

class AbstractAlbumCollectionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        AlbumRole,
        ArtistRole,
        IsValidRole,
        IsLoadedRole,
        IsAvailableRole,
        NameRole,
        YearRole,
        TypeRole,
        SmallCoverUriRole,
        NormalCoverUriRole,
        LargeCoverUriRole,
        ArtistNameRole
    };

    explicit AbstractAlbumCollectionModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;
    
protected:
    virtual Spotinetta::Album getAlbumAt(int index) const = 0;
    virtual int getAlbumCount() const = 0;

    void updateData(int first, int last = -1);

private:
    Q_DISABLE_COPY(AbstractAlbumCollectionModel)  
};

}
