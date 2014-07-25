import QtQuick 2.3
import Sonetta 0.1

import "../common"
import "./playlists"

FocusScope {
    id: root
    property int index: 0
    signal playlistRequested(var index)

    readonly property int topMargin: UI.pageTopMargin + UI.globalSpacing

    states: [
        State {
            name: "container"
            PropertyChanges { target: containerSection; opacity: 1 }
            PropertyChanges { target: playlistSection; opacity: 0 }
            PropertyChanges { target: container; focus: true }
        },
        State {
            name: "playlist"
            PropertyChanges { target: playlistSection; opacity: 1 }
            PropertyChanges { target: containerSection; opacity: 0 }
            PropertyChanges { target: playlistView; focus: true}
            //PropertyChanges { target: playlistView; currentIndex: 0 }
            PropertyChanges { target: playlistModel; playlist: containerModel.playlistAt(root.index) }
        }
    ]

    transitions: [
        Transition {
            from: "container"
            to: "playlist"
            ColorAnimation { duration: UI.playlistPage.fadeTime }
            SequentialAnimation {
                SmoothedAnimation { targets: [container]; property: "opacity"; duration: UI.playlistPage.fadeTime / 2; velocity: -1 }
                SmoothedAnimation { targets: [playlistSection]; property: "opacity"; duration: UI.playlistPage.fadeTime / 2; velocity: -1 }
            }
        },
        Transition {
            from: "playlist"
            to: "container"
            ColorAnimation { duration: UI.playlistPage.fadeTime }
            SequentialAnimation {
                SmoothedAnimation { targets: [playlistSection]; property: "opacity"; duration: UI.playlistPage.fadeTime / 2; velocity: -1 }
                SmoothedAnimation { targets: [container]; property: "opacity"; duration: UI.playlistPage.fadeTime / 2; velocity: -1 }
            }
        }
    ]

    Section {
        id: selectedPlaylistSection
        header: "Selected Playlist"
        color: UI.colors.darkLabel
        anchors {
            top: root.top
            left: root.left
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }
        contentWidth: selectedPlaylist.width
        contentHeight: selectedPlaylist.height

        PlaylistInformation {
            id: selectedPlaylist
            width: 350
            playlist: container.currentPlaylist
        }
    }

    Section {
        id: containerSection
        header: "Your Playlists"
        showFrame: false
        padding: 0
        anchors {
            top: root.top
            left: selectedPlaylistSection.right
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }

        PlaylistContainer {
            id: container
            anchors.fill: parent
            visible: opacity != 0
            model: containerModel

            onPlaylistRequested: root.playlistRequested(index)
        }
    }

    Section {
        id: playlistSection
        header: playlistModel.name
        showFrame: false
        padding: 0
        visible: opacity != 0
        anchors {
            top: root.top
            left: selectedPlaylistSection.right
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }

        TrackView {
            id: playlistView
            anchors.fill: parent
            visible: opacity != 0
            model: playlistModel
            context: playlistModel.playlist
        }
    }

    PlaylistContainerModel {
        id: containerModel
        playlistContainer: session.playlistContainer
    }

    PlaylistModel {
        id: playlistModel
    }
}
