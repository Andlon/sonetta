#ifndef SPALBUMBROWSEWATCHER_H
#define SPALBUMBROWSEWATCHER_H

#include "spalbumbrowse.h"
#include <QObject>

class SpAlbumBrowseWatcher : public QObject {
    Q_OBJECT

public:
    explicit SpAlbumBrowseWatcher(QObject * parent = 0);

    SpAlbumBrowse albumBrowse() const;
    void setAlbumBrowse(const SpAlbumBrowse &albumBrowse);

signals:
    void albumBrowseChanged();
    void loaded();

private slots:
    void handleAlbumBrowseUpdated(const SpAlbumBrowse &browse);

private:
    Q_DISABLE_COPY(SpAlbumBrowseWatcher)
    SpAlbumBrowse m_browse;
};

#endif // SPALBUMBROWSEWATCHER_H
