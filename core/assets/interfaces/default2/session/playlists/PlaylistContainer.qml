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
        onHeightChanged: root.delegateHeight = height

        states: [
            State {
                when: delegateRoot.activeFocus
                PropertyChanges { target: filter; desaturation: 0.0 }
            },
            State {
                when: !delegateRoot.activeFocus
                PropertyChanges { target: filter; desaturation: 1.0 }
            }
        ]

        transitions: Transition {
            SmoothedAnimation { property: "desaturation"; duration: UI.timing.highlightMove; velocity: -1 }
        }

        Mosaic {
            id: mosaic
            width: UI.playlistPage.mosaicSize
            height: UI.playlistPage.mosaicSize
            playlist: delegateRoot.playlist
            placeholder: "../../images/disc_placeholder_small.png"

            anchors {
                left: parent.left
                top: parent.top
                margins: UI.globalSpacing / 2
            }
        }

        Desaturate {
            id: filter
            anchors.fill: mosaic
            source: mosaic
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
