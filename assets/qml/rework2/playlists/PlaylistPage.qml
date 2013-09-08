import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

CollectionView {
    id: root
    model: playlist

    PlaylistModel {
        id: playlist
    }

    delegate: CollectionDelegate {
        height: 100
        width: view.width

        property string name: model ? model.name : ""

        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                leftMargin: ui.misc.globalPadding
                rightMargin: ui.misc.globalPadding
            }

            text: name
            color: ui.colors.standard
            font: ui.fonts.h3
            elide: Text.ElideRight
        }
    }

    Connections {
        target: ui
        onStateChanged: {
            var state = ui.state

            if (state.playlists.playlist !== undefined)
                playlist.playlist = state.playlists.playlist
        }
    }

    onItemPressed: player.play(data.track)
}
