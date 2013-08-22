#ifndef SPOTIFYENUM_H
#define SPOTIFYENUM_H

#include <QObject>

/**
 * @brief Contains enumerations used throughout libsonata.
 *  The Spotify class is essentially a dummy object which enables the enums to be used in QML.
 */


class Spotify : public QObject
{
    Q_OBJECT
    Q_ENUMS(ConnectionState)
    Q_ENUMS(PlaybackState)
    Q_ENUMS(ImageSize)
    Q_ENUMS(ArtistBrowseType)
public:
    explicit Spotify(QObject *parent = 0) : QObject(parent) { }
    
    enum ConnectionState {
        LoggedOutState = 0,
        LoggedInState = 1,
        DisconnectedState = 2,
        UndefinedState = 3,
        OfflineState = 4
    };

    enum PlaybackState {
        PlayState,
        PausedState,
        StoppedState
    };

    enum ImageSize {
        ImageSizeLarge,
        ImageSizeNormal,
        ImageSizeSmall
    };

    enum ArtistBrowseType {
        ArtistBrowseNoTracks,
        ArtistBrowseNoAlbums
    };

    enum AlbumType {
        AlbumAlbumType,
        SingleAlbumType,
        CompilationAlbumType,
        UnknownAlbumType
    };

    enum LinkType {
        InvalidLink = 0,
        TrackLink = 1,
        AlbumLink = 2,
        ArtistLink = 3,
        SearchLink = 4,
        PlaylistLink = 5,
        ProfileLink = 6,
        StarredLink = 7,
        LocaltrackLink = 8,
        ImageLink = 9
    };

    enum SearchType {
        StandardSearch,
        SuggestSearch
    };
};
#endif // SPOTIFYENUM_H
