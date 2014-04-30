import QtQuick 2.2
import Sonetta 0.1
import "States.js" as States

CollectionView {
    id: view

    delegate: CollectionDelegate {
        id: delegateRoot
        height: 180

        states: [
            State {
                when: activeFocus
                PropertyChanges { target: delegateRoot; textColor: ui.colors.highlightText }
            },
            State {
                when: !activeFocus
                PropertyChanges { target: delegateRoot; textColor: ui.colors.standard }
            }
        ]

        transitions: Transition {
            ColorAnimation {
                duration: ui.misc.globalAnimationTime
                easing.type: Easing.InOutQuad
            }
        }

        property string name: model ? model.name : ""
        property url normalCoverUri: model ? model.normalCoverUri : ""
        property int year: model ? model.year : 0
        property string artistName: model ? model.artistName : ""
        property alias textColor: title.color

        SpotifyImage {
            id: cover
            uri: normalCoverUri
            placeholder: "../images/artist_placeholder.png"

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
                id: title
                color: textColor
                text: name + (year > 0 ? " (" + year + ")" : "")
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

            H4 {
                id: artist
                color: textColor
                text: artistName
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
        }
    }

    onItemPressed: UI.push("explore", States.createAlbumBrowseParameters(data.album))
}
