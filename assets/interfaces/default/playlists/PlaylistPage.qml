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
            top: headerLabel.bottom
            topMargin: ui.misc.globalPadding
        }

        width: 60
        rotation: 180
    }

    H2 {
        id: headerLabel
        anchors {
            top: parent.top
            left: view.left
            leftMargin: ui.misc.globalPadding
        }

        height: implicitHeight
        color: ui.colors.label
        text: "Playlist "
    }

    H2 {
        id: header
        anchors {
            top: parent.top
            left: headerLabel.right
            right: view.right
            leftMargin: ui.misc.globalPadding
            rightMargin: ui.misc.globalPadding
        }

        text: playlistModel.name
        height: implicitHeight
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
            top: header.bottom
            topMargin: ui.misc.globalPadding
        }

        onTrackPlayed: player.queue.updateContext(playlistModel.playlist, modelIndex)

        Keys.forwardTo: Nav { onLeft: UI.pop() }
    }

    PlaylistModel {
        id: playlistModel

        onPlaylistChanged: view.currentIndex = 0
    }
}
