#ifndef SPALBUMWATCHER_H
#define SPALBUMWATCHER_H

#include "spalbum.h"
#include <QObject>

class SpAlbumWatcher : public QObject {
Q_OBJECT

public:
    explicit SpAlbumWatcher(QObject * parent = 0);
    SpAlbumWatcher(const SpAlbum &album, QObject * parent = 0);

    SpAlbum album() const;
    void setAlbum(const SpAlbum &album);

signals:
    void albumChanged();
    void loaded();

private slots:
    void handleAlbumLoaded(const SpAlbum &album);

private:
    Q_DISABLE_COPY(SpAlbumWatcher)
    SpAlbum m_album;
};

#endif // SPALBUMWATCHER_H
