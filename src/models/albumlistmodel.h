#pragma once

#include "abstractalbumcollectionmodel.h"
#include <Spotinetta/Spotinetta>

namespace Sonetta {

/*!
 * \brief The AlbumListModel class provides a simple model of an
 *  Spotinetta::AlbumList. Connect a signal to the updateMetadata() slot
 *  to automatically keep up to date.
 */

class AlbumListModel : public AbstractAlbumCollectionModel
{
    Q_OBJECT

public:
    explicit AlbumListModel(QObject * parent = 0);

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    void append(const Spotinetta::AlbumList &albums);
    void clear();

signals:
    void needMore();

protected:
    Spotinetta::Album getAlbumAt(int index) const;
    int getAlbumCount() const;

private:
    Q_DISABLE_COPY(AlbumListModel)
    Spotinetta::AlbumList m_albums;
};

}
