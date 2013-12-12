import QtQuick 2.2
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

CollectionView {
    id: view
    model: container

    PlaylistContainerModel {
        id: container
        playlistContainer: session.playlistContainer
    }

    delegate: CollectionDelegate {
        id: delegate
        height: 120
        width: view.width

        property string name: model ? model.name : ""
        property var playlist: model ? model.playlist : null

        onPlaylistChanged: if (playlist) mosaic.playlist = playlist

        Mosaic {
            id: mosaic
            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
                margins: ui.misc.globalPadding / 2
            }

            width: height
        }

        Text {
            anchors {
                verticalCenter: parent.verticalCenter
                left: mosaic.right
                right: parent.right
                leftMargin: ui.misc.globalPadding
                rightMargin: ui.misc.globalPadding
            }

            text: name
            color: (delegate.activeFocus && isCurrentItem) ? ui.colors.highlightText : ui.colors.standard
            font: ui.fonts.h3
            elide: Text.ElideRight

            Behavior on color {
                ColorAnimation { duration: ui.misc.globalAnimationTime; easing.type: Easing.InOutQuint }
            }
        }
    }

    onItemPressed: {
        var state = ui.state
        state.playlists.stage = "playlist"
        state.playlists.playlist = data.playlist
        ui.pushState(state)
    }
}
