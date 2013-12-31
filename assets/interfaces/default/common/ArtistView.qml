import QtQuick 2.2
import Sonetta 0.1

import "../common/States.js" as States

CollectionView {
    id: view

    delegate: CollectionDelegate {
        height: 180

        states: [
            State {
                when: activeFocus
                PropertyChanges { target: artistName; color: ui.colors.highlightText }
            },
            State {
                when: !activeFocus
                PropertyChanges { target: artistName; color: ui.colors.standard }
            }
        ]

        transitions: Transition {
            ColorAnimation {
                duration: ui.misc.globalAnimationTime
                easing.type: Easing.InOutQuad
            }
        }

        property string name: model ? model.name : ""
        property url normalPortraitUri: model ? model.normalPortraitUri : ""

        SpotifyImage {
            id: portrait
            uri: normalPortraitUri
            placeholder: "../images/artist_placeholder.png"
            //source: "../images/artist_placeholder.png"

            fillMode: Image.PreserveAspectCrop
            width: height

            anchors {
                left: parent.left
                leftMargin: ui.misc.globalPadding / 2
                top: parent.top
                bottom: parent.bottom
                margins: ui.misc.globalPadding
            }
        }

        H3 {
            id: artistName
            anchors {
                left: portrait.right
                leftMargin: ui.misc.globalPadding
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            text: name
        }

    }

    onItemPressed: UI.push("explore", States.createArtistBrowseParameters(data.artist))
}
