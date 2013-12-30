#pragma once

#include <QObject>
#include <QStringList>
#include <Spotinetta/Spotinetta>

#include "../utilities/pointers.h"

namespace Sonetta {

class MosaicGenerator : public QObject {
Q_OBJECT

    Q_PROPERTY(Spotinetta::Playlist playlist READ playlist WRITE setPlaylist NOTIFY playlistChanged)
    Q_PROPERTY(QStringList mosaic READ mosaic NOTIFY mosaicChanged)
public:
    explicit MosaicGenerator(ObjectSharedPointer<const Spotinetta::Session> session, QObject * parent = 0);

    QStringList mosaic() const;

    Spotinetta::Playlist playlist() const;
    void setPlaylist(const Spotinetta::Playlist & playlist);

signals:
    void playlistChanged();
    void mosaicChanged();

private:
    void onPlaylistStateChanged();
    void resetMosaic();
    void updateMosaic();

    QStringList m_mosaic;

    Spotinetta::PlaylistWatcher * m_watcher;
};

}
