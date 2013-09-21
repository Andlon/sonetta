import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

TrackView {
    id: root
    model: playlist

    PlaylistModel {
        id: playlist

        onPlaylistChanged: root.currentIndex = 0
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
}
