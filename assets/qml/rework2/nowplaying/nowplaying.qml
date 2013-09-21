import QtQuick 2.1
import Sonetta 0.1
import "../common"

FocusScope {
    focus: true

    TrackInfo {
        id: track
        track: player.track
    }

    SpotifyImage {
        anchors {
            left: parent.left
            right: view.left
            top: parent.top
            rightMargin: ui.misc.globalPadding
        }

        uri: track.largeCoverUri
    }

    CollectionView {
        id: view

        model: player.queue
        focus: true
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            left: parent.horizontalCenter
        }

        delegate: CollectionDelegate {
            height: 100
            width: view.width

            property string name: model ? model.name : ""
            property string artists: model ? model.artistNames.join(", ") : ""

            Column {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    right: parent.right
                    leftMargin: ui.misc.globalPadding
                    rightMargin: ui.misc.globalPadding
                }

                Text {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    text: name
                    color: ui.colors.standard
                    font: ui.fonts.h4
                    elide: Text.ElideRight
                }

                Text {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    text: artists
                    color: ui.colors.standard
                    font: ui.fonts.standard
                    elide: Text.ElideRight
                }
            }
        }

        //onItemPressed: player.play(data.track)
    }
}
