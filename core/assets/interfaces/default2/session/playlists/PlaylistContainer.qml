import QtQuick 2.3
import Sonetta 0.1

import QtGraphicalEffects 1.0

import "../../common"

CollectionView {
    id: root
    signal playlistRequested(var index)
    property var currentPlaylist: currentItem && currentItem.internalModel ? currentItem.internalModel.playlist : undefined

    onItemPressed: playlistRequested(index)
    delegate: Item {
        id: delegateRoot
        height: mosaic.height + UI.globalSpacing
        focus: true

        property var playlist: model ? model.playlist : undefined
        onPlaylistChanged: if (playlist) mosaic.playlist = playlist
        onHeightChanged: root.delegateHeight = height

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

        Desaturate {
            anchors.fill: mosaic
            source: mosaic
            desaturation: delegateRoot.activeFocus ? 0.0 : 1.0
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
