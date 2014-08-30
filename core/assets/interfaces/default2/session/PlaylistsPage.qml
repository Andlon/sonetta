import QtQuick 2.3
import Sonetta 0.1

import "../common"
import "./playlists"

FocusScope {
    id: root
    property int index: 0
    signal playlistRequested(var index)

    signal albumBrowseRequested(var album)
    signal artistBrowseRequested(var artist)

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
            right: containerSection.left
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }
        contentHeight: selectedPlaylist.height

        PlaylistInformation {
            id: selectedPlaylist
            playlist: container.currentPlaylist
            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }

    FramelessSection {
        id: containerSection
        header: "Your Playlists"
        visible: opacity != 0
        width: UI.playlistPage.listFraction * root.width
        anchors {
            top: root.top
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }

        PlaylistContainer {
            id: container
            anchors.fill: parent
            model: containerModel

            onPlaylistRequested: root.playlistRequested(index)
        }
    }

    FramelessSection {
        id: playlistSection
        header: playlistModel.name
        visible: opacity != 0
        width: UI.playlistPage.listFraction * root.width
        anchors {
            top: root.top
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }

        TrackView {
            id: playlistView
            anchors.fill: parent
            model: playlistModel
            context: playlistModel.playlist

            onArtistBrowseRequested: root.artistBrowseRequested(artist)
            onAlbumBrowseRequested: root.albumBrowseRequested(album)
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
