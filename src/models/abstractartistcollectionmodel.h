#pragma once

#include <QAbstractListModel>

#include <Spotinetta/Spotinetta>

namespace Sonetta {

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
    virtual Spotinetta::Artist getArtistAt(int index) const = 0;
    virtual int getArtistCount() const = 0;

    void updateData(int first, int last = -1);

private:
    Q_DISABLE_COPY(AbstractArtistCollectionModel)
};

}
