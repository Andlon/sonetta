import QtQuick 2.3
import Sonetta 0.1

import "../../common"

CollectionView {
    delegate: Item {
        id: delegateRoot
        height: mosaic.height + UI.globalSpacing
        focus: true

        property var playlist: model ? model.playlist : undefined
        onPlaylistChanged: if (playlist) mosaic.playlist = playlist

        Mosaic {
            id: mosaic
            width: UI.playlistContainer.mosaicSize
            height: UI.playlistContainer.mosaicSize

            anchors {
                left: parent.left
                top: parent.top
                margins: UI.globalSpacing / 2
            }
        }

        FocusText {
            text: model ? model.name : ""
            color: delegateRoot.activeFocus ? UI.colors.focusText : UI.colors.text
            font: UI.fonts.standard
            elide: Text.ElideRight
            hasFocus: delegateRoot.activeFocus

            anchors {
                verticalCenter: mosaic.verticalCenter
                left: mosaic.right
                right: parent.right
                margins: UI.globalSpacing
            }
        }
    }
}
