#ifndef ARTISTLISTMODEL_H
#define ARTISTLISTMODEL_H

#include "abstractartistcollectionmodel.h"

class ArtistListModel : public AbstractArtistCollectionModel
{
    Q_OBJECT
public:
    explicit ArtistListModel(QObject *parent = 0);

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    void append(const SpArtistList &artists);
    void clear();

signals:
    void needMore();

protected:
    SpArtist getArtistAt(int index) const;
    int getArtistCount() const;

private:
    Q_DISABLE_COPY(ArtistListModel)
    SpArtistList m_artists;
};

#endif // ARTISTLISTMODEL_H
