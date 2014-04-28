#pragma once

#include <QObject>
#include <QStringList>
#include <Spotinetta/Spotinetta>

#include "../utilities/pointers.h"

namespace Sonetta {

class MosaicGenerator : public QObject {
Q_OBJECT
    Q_ENUMS(ImageSize)

    Q_PROPERTY(Spotinetta::Playlist playlist READ playlist WRITE setPlaylist NOTIFY playlistChanged)
    Q_PROPERTY(QStringList mosaic READ mosaic NOTIFY mosaicChanged)
    Q_PROPERTY(ImageSize singleCoverSize READ singleCoverSize NOTIFY singleCoverSizeChanged)
    Q_PROPERTY(ImageSize collageCoverSize READ collageCoverSize NOTIFY collageCoverSizeChanged)

public:
    enum ImageSize {
        Small = Spotinetta::ImageSize::Small,
        Normal = Spotinetta::ImageSize::Normal,
        Large = Spotinetta::ImageSize::Large
    };

    explicit MosaicGenerator(ObjectSharedPointer<const Spotinetta::Session> session, QObject * parent = 0);

    QStringList mosaic() const;

    Spotinetta::Playlist playlist() const;
    void setPlaylist(const Spotinetta::Playlist & playlist);

    ImageSize singleCoverSize() const;
    void setSingleCoverSize(ImageSize size);

    ImageSize collageCoverSize() const;
    void setCollageCoverSize(ImageSize size);

signals:
    void playlistChanged();
    void mosaicChanged();
    void singleCoverSizeChanged();
    void collageCoverSizeChanged();

private:
    void updateMosaic();

    QStringList m_mosaic;
    Spotinetta::ImageSize m_singleSize;
    Spotinetta::ImageSize m_collageSize;
    Spotinetta::PlaylistWatcher * m_watcher;
};

}
