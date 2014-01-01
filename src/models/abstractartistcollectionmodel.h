#pragma once

#include <QAbstractListModel>
#include <QPersistentModelIndex>
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

public slots:
    void updateMetadata();
    
protected:
    virtual Spotinetta::Artist getArtistAt(int index) const = 0;
    virtual int getArtistCount() const = 0;

private slots:
    void onRowsInserted(const QModelIndex & parent, int start, int end);
    void onModelReset();

private:
    void checkLoadStatus(int start, int end);
    QVector<QPersistentModelIndex> m_pending;

    Q_DISABLE_COPY(AbstractArtistCollectionModel)
};

}
