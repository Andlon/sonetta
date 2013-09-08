import QtQuick 2.1
import Sonetta 0.1

import "../common"

Item {

    PlaylistContainerModel {
        id: container
        playlistContainer: session.playlistContainer
    }

    CollectionView {
        id: view
        model: container
        anchors.fill: parent
        focus: true

        delegate: CollectionDelegate {
            height: 75
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
            }
        }
    }
}
