import QtQuick 2.1
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
            color: (view.activeFocus && isCurrentItem) ? ui.colors.highlightText : ui.colors.standard
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
