.pragma library

/*
 A collection of functions for generating common state dictionaries
*/

function createLogin()
{
    var state = {};
    state.type = "page";
    state.page = "login";

    return state;
}

function createHome()
{
    var state = {};
    state.type = "page";
    state.page = "home";

    return state;
}

function createPlaylists()
{
    var state = {};
    state.type = "page";
    state.page = "playlists";

    return state;
}

function createArtistBrowse(artist)
{
    var state = {};
    state.type = "page"
    state.page = "artistbrowse"
    state.artist = artist;

    return state;
}

function createAlbumBrowse(album)
{
    var state = {};
    state.type = "page"
    state.page = "albumbrowse"
    state.album = album;

    return state;
}
