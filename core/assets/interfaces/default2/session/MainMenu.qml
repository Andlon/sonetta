import QtQuick 2.3
import Navigation 0.1

import "../common"

Menu {
    id: root
    signal playbackRequested()
    signal playlistsRequested()
    signal exploreRequested()
    signal settingsRequested()

    navigationEnabled: false

    states: [
        State {
            name: "playback"
            PropertyChanges { target: root; currentIndex: 0 }
            PropertyChanges { target: browse; opacity: 0 }
        },
        State {
            name: "playlists"
            PropertyChanges { target: root; currentIndex: 1 }
            PropertyChanges { target: browse; opacity: 0 }
        },
        State {
            name: "explore"
            PropertyChanges { target: root; currentIndex: 2 }
            PropertyChanges { target: browse; opacity: 0 }
        },
        State {
            name: "settings"
            PropertyChanges { target: root; currentIndex: 3 }
            PropertyChanges { target: browse; opacity: 0 }
        },
        State {
            name: "browse"
            PropertyChanges { target: root; currentIndex: 4 }
            PropertyChanges { target: browse; opacity: 1 }
        }
    ]

    transitions: Transition {
        SmoothedAnimation { property: "opacity"; duration: root.highlightMoveDuration; velocity: -1 }
    }

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

    MainMenuItem {
        id: browse
        label: "Browse"
        visible: opacity !== 0
        Navigation.onUp: settingsRequested()
    }
}
