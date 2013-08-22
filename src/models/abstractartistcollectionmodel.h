#ifndef ABSTRACTARTISTCOLLECTIONMODEL_H
#define ABSTRACTARTISTCOLLECTIONMODEL_H

#include <QAbstractListModel>
#include <Sonata/SpArtist>

class AbstractArtistCollectionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        ArtistRole = Qt::UserRole,
        IsValidRole,
        IsLoadedRole,
        NameRole,
        SmallPortraitUriRole,
        NormalPortraitUriRole,
        LargePortraitUriRole
    };

    explicit AbstractArtistCollectionModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    QHash<int, QByteArray> roleNames() const;
    
protected:
    virtual SpArtist getArtistAt(int index) const = 0;
    virtual int getArtistCount() const = 0;

    void updateData(int first, int last = -1);

private:
    Q_DISABLE_COPY(AbstractArtistCollectionModel)
};

#endif // ABSTRACTARTISTCOLLECTIONMODEL_H
