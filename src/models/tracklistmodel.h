#ifndef TRACKLISTMODEL_H
#define TRACKLISTMODEL_H

#include "abstracttrackcollectionmodel.h"

class TrackListModel : public AbstractTrackCollectionModel
{
    Q_OBJECT

public:
    explicit TrackListModel(QObject * parent);

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    void append(const SpTrackList &tracks);
    void clear();

signals:
    void needMore();

protected:
    SpTrack getTrackAt(int index) const;
    int getTrackCount() const;

private:
    Q_DISABLE_COPY(TrackListModel)
    SpTrackList m_tracks;
};

#endif // TRACKLISTMODEL_H
