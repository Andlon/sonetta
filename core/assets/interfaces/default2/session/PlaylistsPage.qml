import QtQuick 2.3
import Sonetta 0.1

import "../common"
import "./playlists"

FocusScope {
    id: root
    property int index: 0
    signal playlistRequested(var index)

    states: [
        State {
            name: "container"
            PropertyChanges { target: container; visible: true; focus: true }
            PropertyChanges { target: playlistView; visible: false; focus: false }
            PropertyChanges { target: header; color: UI.colors.label }
            PropertyChanges { target: playlistHeader; opacity: 0 }
        },
        State {
            name: "playlist"
            PropertyChanges { target: container; visible: false; focus: false }
            PropertyChanges { target: playlistView; visible: true; focus: true; }
            PropertyChanges { target: playlistModel; playlist: container.model.playlistAt(root.index) }
            PropertyChanges { target: header; color: UI.colors.darkLabel }
            PropertyChanges { target: playlistHeader; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            ColorAnimation { duration: UI.timing.highlightMove }
            OpacityAnimator { target: header; duration: UI.timing.highlightMove }
        }
    ]

    Text {
        id: header
        font: UI.fonts.mainMenu
        text: "Your Playlists"
        anchors {
            top: root.top
            left: root.left
            margins: UI.globalSpacing
        }
    }

    Text {
        id: playlistHeader
        font: UI.fonts.mainMenu
        color: UI.colors.label
        text: playlistModel.name
        anchors {
            top: root.top
            left: header.right
            right: root.right
            margins: UI.globalSpacing
        }
    }

    PlaylistContainer {
        id: container
        focus: true
        model: PlaylistContainerModel {
            playlistContainer: session.playlistContainer
            onLoadedChanged: playlistModel.playlist = playlistAt(root.index)
        }
        anchors {
            top: header.bottom
            left: root.left
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
        }

        onPlaylistRequested: root.playlistRequested(index)
    }

    PlaylistView {
        id: playlistView
        model: PlaylistModel {
            id: playlistModel
            onPlaylistChanged: playlistView.currentIndex = 0
        }
        focus: true
        anchors {
            top: header.bottom
            left: root.left
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
        }

        onItemPressed: {
            player.play(model.track)
            player.queue.updateContext(playlistModel.playlist, index)
        }
    }

}
