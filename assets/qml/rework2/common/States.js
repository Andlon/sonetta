.pragma library

function createInitialState()
{
    var state = {}

    // Determines
    state.page = "playlists"
    state.showTopSection = true

    state.playlists = {}

    // Holds the current Spotinetta::Playlist object, or undefined if none
    state.playlists.playlist = undefined

    // Possible stages:
    // "container": displays a list of available playlists for the current user
    // "playlist": displays the playlist specified by the above playlist property
    state.playlists.stage = "list"

    return state
}
