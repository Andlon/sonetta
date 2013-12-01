#include "mosaicgenerator.h"

#include <QSet>

namespace sp = Spotinetta;

namespace Sonetta {

namespace {

const unsigned int PREFERRED_TILECOUNT = 4;

sp::AlbumList analyzePlaylist(const Spotinetta::Playlist & playlist, unsigned int preferredTileCount)
{
    sp::AlbumList albums;

    for (sp::Track & track : playlist.tracks())
    {
        // For debugging purposes
        Q_ASSERT(track.isLoaded());
        Q_ASSERT(track.album().isLoaded());

        const sp::Album album = track.album();

        if (!albums.contains(album))
        {
            albums.append(album);
        }

        if (albums.count() == static_cast<int>(preferredTileCount))
            break;
    }

    return albums;
}

}

MosaicGenerator::MosaicGenerator(const Spotinetta::Session * session, QObject *parent)
    :   QObject(parent), m_watcher(new Spotinetta::PlaylistWatcher(session, this))
{
    connect(m_watcher, &Spotinetta::PlaylistWatcher::stateChanged, this, &MosaicGenerator::onPlaylistStateChanged);
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
    resetMosaic();
    m_watcher->watch(playlist);
    emit playlistChanged();

    if (playlist.isLoaded())
        updateMosaic();
    else
    {
        emit mosaicChanged();
    }
}

void MosaicGenerator::onPlaylistStateChanged()
{
    if (m_mosaic.empty() && m_watcher->watched().isLoaded())
    {
        updateMosaic();
    }
}

void MosaicGenerator::resetMosaic()
{
    m_mosaic.clear();
}

void MosaicGenerator::updateMosaic()
{
    auto albums = analyzePlaylist(m_watcher->watched(), PREFERRED_TILECOUNT);

    for (const sp::Album & album : albums)
    {
        const QString uri = sp::Link::fromAlbumCover(album, sp::ImageSize::Normal).uri();
        m_mosaic.append(uri);
    }
    emit mosaicChanged();
}






}
