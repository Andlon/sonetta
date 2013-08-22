#ifndef ABSTRACTALBUMCOLLECTIONMODEL_H
#define ABSTRACTALBUMCOLLECTIONMODEL_H

#include <QAbstractListModel>
#include <Sonata/SpAlbum>

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
    virtual SpAlbum getAlbumAt(int index) const = 0;
    virtual int getAlbumCount() const = 0;

    void updateData(int first, int last = -1);

private:
    Q_DISABLE_COPY(AbstractAlbumCollectionModel)  
};

#endif // ABSTRACTALBUMCOLLECTIONMODEL_H
