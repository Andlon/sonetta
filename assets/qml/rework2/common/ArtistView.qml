import QtQuick 2.1
import Sonetta 0.1

CollectionView {
    id: view

    delegate: CollectionDelegate {
        height: 180

        property string name: model ? model.name : ""
        property url normalPortraitUri: model ? model.normalPortraitUri : ""

        SpotifyImage {
            id: portrait
            uri: normalPortraitUri
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

        H4 {
            anchors {
                left: portrait.right
                leftMargin: ui.misc.globalPadding
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            text: name
        }

    }
}
