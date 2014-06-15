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
            PropertyChanges { target: playlistView; visible: false; focus: false}
        },
        State {
            name: "playlist"
            PropertyChanges { target: container; visible: false; focus: false }
            PropertyChanges { target: playlistView; visible: true; focus: true; }
            PropertyChanges { target: playlistModel; playlist: container.model.playlistAt(root.index) }
        }
    ]

    PlaylistContainer {
        id: container
        focus: true
        model: PlaylistContainerModel {
            playlistContainer: session.playlistContainer
            onLoadedChanged: playlistModel.playlist = playlistAt(root.index)
        }
        anchors {
            fill: parent
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
            fill: parent
            margins: UI.globalSpacing
        }

        onItemPressed: {
            player.play(model.track)
            player.queue.updateContext(playlistModel.playlist, index)
        }
    }

}
