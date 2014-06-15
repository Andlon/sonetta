import QtQuick 2.3
import Sonetta 0.1

import "../common"
import "./playlists"

FocusScope {

    PlaylistContainer {
        focus: true
        model: PlaylistContainerModel {
            playlistContainer: session.playlistContainer
        }
        anchors {
            fill: parent
            margins: UI.globalSpacing
        }
    }

}
