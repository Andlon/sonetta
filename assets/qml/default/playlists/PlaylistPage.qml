import QtQuick 2.2
import Sonetta 0.1
import Navigation 0.1

import "../common"

FocusScope {
    id: root

    property alias playlist: playlistModel.playlist

    BackIndicator {
        id: back
        anchors {
            left: parent.left
            top: parent.top
            topMargin: ui.misc.globalPadding
        }

        width: 60
        rotation: 180
    }

    TrackView {
        id: view
        model: playlistModel
        focus: true

        anchors {
            left: back.right
            leftMargin: ui.misc.globalPadding
            right: root.right
            bottom: root.bottom
            top: root.top
        }

        PlaylistModel {
            id: playlistModel

            onPlaylistChanged: view.currentIndex = 0
        }

        onTrackPlayed: player.queue.updateContext(playlistModel.playlist, modelIndex)

        Keys.forwardTo: Nav { onLeft: UI.pop() }
    }
}
