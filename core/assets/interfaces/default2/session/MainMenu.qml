import QtQuick 2.3

import "../common"

Menu {
    readonly property string page: {
        switch (currentIndex) {
        case 0:
            return "playback"
        case 1:
            return "playlists"
        case 2:
            return "explore"
        case 3:
            return "settings"
        }
    }

    MainMenuItem {
        id: playback
        label: "Playback"
    }

    MainMenuItem {
        id: playlists
        label: "Playlists"
    }

    MainMenuItem {
        id: explore
        label: "Explore"
    }

    MainMenuItem {
        id: settings
        label: "Settings"
    }
}
