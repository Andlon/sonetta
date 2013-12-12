import QtQuick 2.2
import Sonetta 0.1
import "../common"

FocusScope {
    id: root
    focus: true

    TrackInfo {
        id: track
        track: player.track
    }

    Label {
        id: playingLabel
        text: "NOW PLAYING"
        anchors {
            left: parent.left
            leftMargin: ui.misc.globalPadding
            right: parent.horizontalCenter
            top: parent.top
        }
    }

    SpotifyImage {
        id: cover

        fillMode: Image.PreserveAspectFit
        anchors {
            left: parent.left
            right: view.left
            top: view.top
            bottom: trackInfo.top
            bottomMargin: ui.misc.globalPadding / 2
            rightMargin: ui.misc.globalPadding
        }

        uri: track.largeCoverUri
    }

    Column {
        id: trackInfo
        anchors {
            left: parent.left
            bottom: controlContainer.top
            bottomMargin: ui.misc.globalPadding / 2
            right: cover.right
        }

        height: childrenRect.height

        H3 {
            anchors {
                left: parent.left
                right: parent.right
            }

            horizontalAlignment: Text.AlignHCenter
            text: track.name
        }

        H4 {
            anchors {
                left: parent.left
                right: parent.right
            }

            horizontalAlignment: Text.AlignHCenter
            text: track.artistNames.join(", ")
        }
    }

    Item {
        id: controlContainer
        height: controls.height
        anchors {
            bottom: root.bottom
            left: root.left
            right: cover.right
        }

        Controls {
            id: controls
            anchors {
                centerIn: parent
            }

            focus: true
        }

        Navigation.onRight: view.focus = true
    }

    Label {
        id: queueLabel
        text: "QUEUE"
        anchors {
            left: parent.horizontalCenter
            leftMargin: ui.misc.globalPadding
            right: parent.right
            top: parent.top
        }
    }

    TrackView {
        id: view

        model: player.queue
        anchors {
            top: queueLabel.bottom
            topMargin: ui.misc.globalPadding
            bottom: parent.bottom
            right: parent.right
            left: parent.horizontalCenter
        }

        singleRowLayout: false
        snapMode: ListView.SnapToItem

        onCountChanged: view.currentIndex = 0

        onTrackPlayed: player.queue.pop(modelIndex)

        Navigation.onLeft: controls.focus = true
    }
}
