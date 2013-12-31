.pragma library

function createAlbumBrowseParameters(album)
{
    return { type: "album", album: album }
}

function createArtistBrowseParameters(artist)
{
    return { type: "artist", artist: artist }
}
