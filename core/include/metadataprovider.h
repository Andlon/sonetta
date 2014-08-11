#pragma once

#include "artist.h"
#include "track.h"
#include "album.h"

#include <QObject>
#include <QString>

namespace Sonetta {

class MetadataProvider : public QObject {
public:
    /**
     * @brief fetchTrackData Instruct the provider to make an attempt to fetch meta data for the given track.
     * @param uri The track uri to fetch meta data for.
     * @return Returns true if the URI is valid for the service and the provider will attempt to fetch it, false otherwise.
     */
    virtual bool fetchTrackData(const QString & uri) = 0;

    /**
     * @brief fetchAlbumData Instruct the provider to make an attempt to fetch meta data for the given album.
     * @param uri The album uri to fetch meta data for.
     * @return Returns true if the URI is valid for the service and the provider will attempt to fetch it, false otherwise.
     */
    virtual bool fetchAlbumData(const QString & uri) = 0;

    /**
     * @brief fetchArtistData Instruct the provider to make an attempt to fetch meta data for the given artist.
     * @param uri The artist uri to fetch meta data for.
     * @return Returns true if the URI is valid for the service and the provider will attempt to fetch it, false otherwise.
     */
    virtual bool fetchArtistData(const QString & uri) = 0;

signals:
    void trackFetched(TrackData track);
    void albumFetched(AlbumData album);
    void artistFetched(ArtistData artist);

    void trackFetchFailed(TrackData track);
    void albumFetchFailed(AlbumData album);
    void artistFetchFailed(ArtistData artist);
};
}
