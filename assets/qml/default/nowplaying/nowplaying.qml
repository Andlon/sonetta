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
        verticalAlignment: Image.AlignTop

        anchors {
            left: parent.left
            right: view.left
            top: view.top
            bottom: playback.top
            bottomMargin: ui.misc.globalPadding / 2
            rightMargin: ui.misc.globalPadding
        }

        uri: track.largeCoverUri
    }

    Playback {
        id: playback
        anchors {
            left: parent.left
            bottom: trackbar.top
            bottomMargin: ui.misc.globalPadding
            right: cover.right
        }

        focus: true

        height: childrenRect.height
        trackName: track.name
        artistName: track.artistNames.join(", ")

        Navigation.onRight: view.focus = true
        Navigation.onDown: trackbar.focus = true
    }

    Trackbar {
        id: trackbar
        anchors {
            bottom: parent.bottom
            left: playback.left
            right: playback.right
        }

        Navigation.onUp: playback.focus = true
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

        Navigation.onLeft: playback.focus = true
    }
}
