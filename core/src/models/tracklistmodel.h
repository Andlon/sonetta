#pragma once

#include "abstracttrackcollectionmodel.h"

namespace Sonetta {

class TrackListModel : public AbstractTrackCollectionModel
{
    Q_OBJECT

    Q_PROPERTY(Spotinetta::TrackList list READ list NOTIFY listChanged)

public:
    explicit TrackListModel(ObjectSharedPointer<const Spotinetta::Session> session, QObject * parent);

    bool canFetchMore(const QModelIndex &parent) const;
    void fetchMore(const QModelIndex &parent);

    void append(const Spotinetta::TrackList &tracks);
    void clear();

    Spotinetta::TrackList list() const;

signals:
    void listChanged();
    void needMore();

protected:
    Spotinetta::Track getTrackAt(int index) const;
    int getTrackCount() const;

private:
    Q_DISABLE_COPY(TrackListModel)
    Spotinetta::TrackList m_tracks;
};

}
