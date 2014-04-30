#pragma once

#include "abstractartistcollectionmodel.h"

namespace Sonetta {

class ArtistListModel : public AbstractArtistCollectionModel
{
    Q_OBJECT
public:
    explicit ArtistListModel(QObject *parent = 0);

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    void append(const Spotinetta::ArtistList &artists);
    void clear();

signals:
    void needMore();

protected:
    Spotinetta::Artist getArtistAt(int index) const;
    int getArtistCount() const;

private:
    Q_DISABLE_COPY(ArtistListModel)
    Spotinetta::ArtistList m_artists;
};

}
