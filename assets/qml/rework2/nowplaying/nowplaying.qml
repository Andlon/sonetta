import QtQuick 2.1
import Sonetta 0.1
import "../common"

FocusScope {
    focus: true

    TrackInfo {
        id: track
        track: player.track
    }

    Label {
        id: playingLabel
        text: "NOW PLAYING:"
        anchors {
            left: parent.left
            leftMargin: ui.misc.globalPadding
            right: parent.horizontalCenter
            top: parent.top
        }
    }

    SpotifyImage {
        id: cover
        anchors {
            left: parent.left
            right: view.left
            top: view.top
            rightMargin: ui.misc.globalPadding
        }

        uri: track.largeCoverUri
    }

    Column {
        anchors {
            left: parent.left
            top: cover.bottom
            topMargin: ui.misc.globalPadding
            right: cover.right
        }

        height: childrenRect.height

        H2 {
            anchors {
                left: parent.left
                right: parent.right
            }

            horizontalAlignment: Text.AlignHCenter
            text: track.name
        }

        H3 {
            anchors {
                left: parent.left
                right: parent.right
            }

            horizontalAlignment: Text.AlignHCenter
            text: track.artistNames.join(",")
        }
    }

    Label {
        id: queueLabel
        text: "QUEUE:"
        anchors {
            left: parent.horizontalCenter
            leftMargin: ui.misc.globalPadding
            right: parent.right
            top: parent.top
        }
    }

    CollectionView {
        id: view

        model: player.queue
        focus: true
        anchors {
            top: queueLabel.bottom
            topMargin: ui.misc.globalPadding
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
