import QtQuick 2.1
import Sonetta 0.1

CollectionView {
    id: view

    delegate: CollectionDelegate {
        height: 180

        property string name: model ? model.name : ""
        property url normalCoverUri: model ? model.normalCoverUri : ""
        property int year: model ? model.year : 0
        property string artistName: model ? model.artistName : ""

        SpotifyImage {
            id: cover
            uri: normalCoverUri
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

        Column {
            anchors {
                left: cover.right
                margins: ui.misc.globalPadding
                right: parent.right
                verticalCenter: parent.verticalCenter
            }

            H3 {
                text: name + (year > 0 ? " (" + year + ")" : "")
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

            H4 {
                text: artistName
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

        }

    }
}
