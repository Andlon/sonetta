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
            PropertyChanges { target: container; opacity: 1; focus: true }
            PropertyChanges { target: playlistView; opacity: 0; focus: false }
            PropertyChanges { target: header; opacity: 1 }
            PropertyChanges { target: playlistHeader; opacity: 0 }
        },
        State {
            name: "playlist"
            PropertyChanges { target: container; opacity: 0; focus: false }
            PropertyChanges { target: playlistView; opacity: 1; currentIndex: 0; focus: true; }
            PropertyChanges { target: playlistModel; playlist: container.model.playlistAt(root.index) }
            PropertyChanges { target: header; opacity: 0 }
            PropertyChanges { target: playlistHeader; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "container"
            to: "playlist"
            ColorAnimation { duration: UI.playlistPage.fadeTime }
            SequentialAnimation {
                SmoothedAnimation { targets: [container, header]; property: "opacity"; duration: UI.playlistPage.fadeTime / 2; velocity: -1 }
                SmoothedAnimation { targets: [playlistView, playlistHeader]; property: "opacity"; duration: UI.playlistPage.fadeTime / 2; velocity: -1 }
            }
        },
        Transition {
            from: "playlist"
            to: "container"
            ColorAnimation { duration: UI.playlistPage.fadeTime }
            SequentialAnimation {
                SmoothedAnimation { targets: [playlistView, playlistHeader]; property: "opacity"; duration: UI.playlistPage.fadeTime / 2; velocity: -1 }
                SmoothedAnimation { targets: [container, header]; property: "opacity"; duration: UI.playlistPage.fadeTime / 2; velocity: -1 }
            }
        }
    ]

    Text {
        id: header
        font: UI.fonts.mainMenu
        text: "Your Playlists"
        color: UI.colors.label
        elide: Text.ElideRight
        anchors {
            top: root.top
            left: container.left
            right: container.right
            topMargin: UI.globalSpacing
        }
    }

    Text {
        id: playlistHeader
        font: UI.fonts.mainMenu
        color: UI.colors.label
        text: playlistModel.name
        elide: Text.ElideRight
        anchors {
            top: root.top
            left: container.left
            right: container.right
            topMargin: UI.globalSpacing
        }
    }

    Text {
        id: selectedPlaylistHeader
        font: UI.fonts.mainMenu
        text: "Selected playlist"
        color: UI.colors.darkLabel
        anchors {
            top: header.top
            left: playlistInformation.left
            right: playlistInformation.right
        }
    }

    PlaylistInformation {
        id: playlistInformation
        width: 350
        anchors {
            top: header.bottom
            left: root.left
            bottom: root.bottom
            margins: UI.globalSpacing
        }

        playlist: container.currentPlaylist
    }

    PlaylistContainer {
        id: container
        focus: true
        visible: opacity != 0
        model: PlaylistContainerModel {
            playlistContainer: session.playlistContainer
        }
        anchors {
            top: header.bottom
            left: playlistInformation.right
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
        }

        onPlaylistRequested: root.playlistRequested(index)
    }

    PlaylistView {
        id: playlistView
        focus: true
        visible: opacity != 0
        model: PlaylistModel {
            id: playlistModel
        }
        anchors {
            top: header.bottom
            left: playlistInformation.right
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
