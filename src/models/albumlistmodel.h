#ifndef ALBUMLISTMODEL_H
#define ALBUMLISTMODEL_H

#include "abstractalbumcollectionmodel.h"

#include <Sonata/SpAlbumWatcher>

/*!
 * \brief The AlbumListModel class provides a simple model of an
 *  SpAlbumList where the albums have already been loaded.
 */

class AlbumListModel : public AbstractAlbumCollectionModel
{
    Q_OBJECT

public:
    explicit AlbumListModel(QObject * parent = 0);

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    void append(const SpAlbumList &albums);
    void clear();

signals:
    void needMore();

protected:
    SpAlbum getAlbumAt(int index) const;
    int getAlbumCount() const;

private:
    Q_DISABLE_COPY(AlbumListModel)
    SpAlbumList m_albums;
};

#endif // ALBUMLISTMODEL_H
