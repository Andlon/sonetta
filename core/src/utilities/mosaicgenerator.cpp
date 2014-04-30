#include "mosaicgenerator.h"

#include <QSet>

namespace sp = Spotinetta;

namespace Sonetta {

namespace {

const int PREFERRED_TILECOUNT = 4;

sp::AlbumList analyzePlaylist(const Spotinetta::Playlist & playlist, unsigned int preferredTileCount)
{
    sp::AlbumList albums;

    for (sp::Track & track : playlist.tracks())
    {
        const sp::Album album = track.album();

        if (!track.isLoaded() || !album.isLoaded())
            break;

        if (!albums.contains(album))
            albums.append(album);

        if (albums.count() == static_cast<int>(preferredTileCount))
            break;
    }

    return albums;
}

}

MosaicGenerator::MosaicGenerator(ObjectSharedPointer<const Spotinetta::Session> session, QObject *parent)
    :   QObject(parent), m_watcher(new Spotinetta::PlaylistWatcher(session.data(), this)),
      m_singleSize(sp::ImageSize::Normal), m_collageSize(sp::ImageSize::Small)
{
    connect(m_watcher, &Spotinetta::PlaylistWatcher::stateChanged, this, &MosaicGenerator::updateMosaic);
    connect(m_watcher, &Spotinetta::PlaylistWatcher::metadataUpdated, this, &MosaicGenerator::updateMosaic);
}

QStringList MosaicGenerator::mosaic() const
{
    return m_mosaic;
}

Spotinetta::Playlist MosaicGenerator::playlist() const
{
    return m_watcher->watched();
}

void MosaicGenerator::setPlaylist(const Spotinetta::Playlist &playlist)
{
    m_mosaic.clear();
    m_watcher->watch(playlist);
    emit playlistChanged();

    if (playlist.isLoaded())
        updateMosaic();
    else
        emit mosaicChanged();
}

MosaicGenerator::ImageSize MosaicGenerator::singleCoverSize() const
{
    return static_cast<MosaicGenerator::ImageSize>(m_singleSize);
}

void MosaicGenerator::setSingleCoverSize(ImageSize size)
{
    m_singleSize = static_cast<Spotinetta::ImageSize>(size);
    emit singleCoverSizeChanged();
}

MosaicGenerator::ImageSize MosaicGenerator::collageCoverSize() const
{
    return static_cast<MosaicGenerator::ImageSize>(m_collageSize);
}

void MosaicGenerator::setCollageCoverSize(ImageSize size)
{
    m_collageSize = static_cast<Spotinetta::ImageSize>(size);
    emit collageCoverSizeChanged();
}

void MosaicGenerator::updateMosaic()
{
    sp::Playlist pl = playlist();
    if (m_mosaic.count() < PREFERRED_TILECOUNT && pl.isLoaded())
    {
        m_mosaic.clear();

        auto albums = analyzePlaylist(pl, PREFERRED_TILECOUNT);
        sp::ImageSize size = albums.count() == 1 ? m_singleSize : m_collageSize;

        for (const sp::Album & album : albums)
        {
            const QString uri = sp::Link::fromAlbumCover(album, size).uri();
            m_mosaic.append(uri);
        }
        emit mosaicChanged();
    }
}

}
