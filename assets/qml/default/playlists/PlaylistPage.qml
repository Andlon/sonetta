import QtQuick 2.2
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

FocusScope {
    id: root

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
        model: playlist
        focus: true

        anchors {
            left: back.right
            leftMargin: ui.misc.globalPadding
            right: root.right
            bottom: root.bottom
            top: root.top
        }

        PlaylistModel {
            id: playlist

            onPlaylistChanged: view.currentIndex = 0
        }

        Connections {
            target: ui
            onStateChanged: {
                var state = ui.state

                if (state.playlists.playlist !== undefined)
                {
                    playlist.playlist = state.playlists.playlist
                }
            }
        }

        onTrackPlayed: player.queue.updateContext(playlist.playlist, modelIndex)

        Navigation.onLeft: ui.popState()
    }
}
