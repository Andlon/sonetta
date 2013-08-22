import QtQuick 2.0
import sonata 0.1

FocusScope {
    height: row.childrenRect.height + 20
    width: row.childrenRect.width

    Row {
        id: row
        spacing: 5
        anchors.fill: parent
        anchors.topMargin: 10
        anchors.bottomMargin: 10

        ImageButton {
            id: prev
            source: "images/prev.png"
            anchors.verticalCenter: playPause.verticalCenter
            focusSource: source

            onClicked: player.prev()
        }

        ImageButton {
            id: playPause
            property bool isPlaying: spotify.playbackState === Spotify.PlayState
            source: isPlaying ? "images/pause.png" : "images/play.png"
            focusSource: source

            onClicked: isPlaying ? spotify.pause() : spotify.play()
        }

        ImageButton {
            id: next
            source: "images/next.png"
            anchors.verticalCenter: playPause.verticalCenter
            focusSource: source

            onClicked: player.next()
        }
    }
}
