.pragma library

function createInitialState()
{
    var state = {}

    /* Generic options */
    state.page = "playlists"
    state.pageLabel = "playlists"
    state.showTopSection = true

    /* Playlists page options */
    state.playlists = {}

    // Holds the current Spotinetta::Playlist object, or undefined if none
    state.playlists.playlist = undefined

    // Possible stages:
    // "container": displays a list of available playlists for the current user
    // "playlist": displays the playlist specified by the above playlist property
    state.playlists.stage = "list"

    return state
}

function createPage(state, page)
{
    state.page = page
    state.pageLabel = getLabel(page)
    state.showTopSection = page !== "nowplaying"

    return state
}

function getLabel(page)
{
    switch (page)
    {
    case "playlists":
        return "playlists"
    case "nowplaying":
        return "now playing"
    case "search":
        return "search"
    case "discover":
        return "discover"
    default:
        return ""
    }
}
