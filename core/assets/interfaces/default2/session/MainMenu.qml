import QtQuick 2.3
import Navigation 0.1

import "../common"

Menu {
    signal playbackRequested()
    signal playlistsRequested()
    signal exploreRequested()
    signal settingsRequested()

    MainMenuItem {
        id: playback
        label: "Playback"
        Navigation.onDown: playlistsRequested()
    }

    MainMenuItem {
        id: playlists
        label: "Playlists"
        Navigation.onUp: playbackRequested()
        Navigation.onDown: exploreRequested()
    }

    MainMenuItem {
        id: explore
        label: "Explore"
        Navigation.onUp: playlistsRequested()
        Navigation.onDown: settingsRequested()
    }

    MainMenuItem {
        id: settings
        label: "Settings"
        Navigation.onUp: exploreRequested()
    }
}
