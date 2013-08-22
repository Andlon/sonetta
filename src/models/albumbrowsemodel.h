#ifndef ALBUMBROWSEMODEL_H
#define ALBUMBROWSEMODEL_H

#include "abstracttrackcollectionmodel.h"
#include <Sonata/SpAlbumBrowseWatcher>

class AlbumBrowseModel : public AbstractTrackCollectionModel
{
    Q_OBJECT

    Q_PROPERTY(SpAlbumBrowse albumBrowse READ albumBrowse WRITE setAlbumBrowse NOTIFY albumBrowseChanged)

public:
    explicit AlbumBrowseModel(QObject * parent = 0);

    SpAlbumBrowse albumBrowse() const;
    void setAlbumBrowse(const SpAlbumBrowse &browse);

protected:
    int getTrackCount() const;
    SpTrack getTrackAt(int index) const;

signals:
    void albumBrowseChanged();

private slots:
    void handleLoad();

private:
    Q_DISABLE_COPY(AlbumBrowseModel)
    SpAlbumBrowseWatcher * m_watcher;
    SpTrackList m_tracks;
};

#endif // ALBUMBROWSEMODEL_H
