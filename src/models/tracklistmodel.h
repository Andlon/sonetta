#pragma once

#include "abstracttrackcollectionmodel.h"

namespace Sonetta {

class TrackListModel : public AbstractTrackCollectionModel
{
    Q_OBJECT

public:
    explicit TrackListModel(QObject * parent);

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    void append(const Spotinetta::TrackList &tracks);
    void clear();

signals:
    void needMore();

protected:
    Spotinetta::Track getTrackAt(int index) const;
    int getTrackCount() const;

private:
    Q_DISABLE_COPY(TrackListModel)
    Spotinetta::TrackList m_tracks;
};

}
