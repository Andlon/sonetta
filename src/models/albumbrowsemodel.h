#pragma once

#include "abstracttrackcollectionmodel.h"

namespace Sonetta {

class AlbumBrowseModel : public AbstractTrackCollectionModel
{
    Q_OBJECT

    Q_PROPERTY(Spotinetta::AlbumBrowse albumBrowse READ albumBrowse WRITE setAlbumBrowse NOTIFY albumBrowseChanged)

public:
    explicit AlbumBrowseModel(Spotinetta::Session * session, QObject * parent = 0);

    Spotinetta::AlbumBrowse albumBrowse() const;
    void setAlbumBrowse(const Spotinetta::AlbumBrowse &browse);

protected:
    int getTrackCount() const;
    Spotinetta::Track getTrackAt(int index) const;

signals:
    void albumBrowseChanged();

private slots:
    void onLoaded();

private:
    Q_DISABLE_COPY(AlbumBrowseModel)
    Spotinetta::AlbumBrowseWatcher *    m_watcher;
    Spotinetta::TrackList               m_tracks;
};

}
