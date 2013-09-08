import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

CollectionView {
    id: root
    model: playlist

    PlaylistModel {
        id: playlist

        onPlaylistChanged: root.currentIndex = 0
    }

    delegate: CollectionDelegate {
        height: 100
        width: view.width

        property string name: model ? model.name : ""
        property string artists: model ? model.artistNames.join(", ") : ""

        Column {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.left
                right: parent.right
                leftMargin: ui.misc.globalPadding
                rightMargin: ui.misc.globalPadding
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                text: name
                color: ui.colors.standard
                font: ui.fonts.h3
                elide: Text.ElideRight
            }

            Text {
                anchors {
                    left: parent.left
                    right: parent.right
                }

                text: artists
                color: ui.colors.standard
                font: ui.fonts.standard
                elide: Text.ElideRight
            }
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
