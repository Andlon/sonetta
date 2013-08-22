import QtQuick 2.0
import com.bitfraction.sonata 0.1

import "time.js" as Time

FocusScope {
    width: 300
    height: childrenRect.height

    readonly property bool isValid: track.isValid

    SpTrackInfo {
        id: track
        track: spotify.currentTrack

        onIsValidChanged: console.log(isValid)
    }

    Item {
        id: trackInfo
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 90

        clip: true

        Column {
            anchors.left: parent.left
            anchors.right: playbackControl.left
            anchors.verticalCenter: playbackControl.verticalCenter

            Text {
                text: "SONG"
                color: Qt.darker(trackName.color, 1.5)
                font.pointSize: 15
                horizontalAlignment: Text.AlignRight
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.rightMargin: 20
                font.bold: true
            }

            Text {
                id: trackName
                anchors.rightMargin: 20
                anchors.right: parent.right
                anchors.left: parent.left
                text: track.name
                color: "#efefef"
                font.pointSize: 22
                font.family: "Roboto"
                font.bold: true
                horizontalAlignment: Text.AlignRight
                elide: Text.ElideRight
            }

        }

        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            //            anchors.verticalCenter: parent.verticalCenter

            id: playbackControl

            ImageButton {
                anchors.verticalCenter: parent.verticalCenter
                id: prevButton
                source: "prev.png"
                focusSource: "prev.png"

                onClicked: sonata.prev()
            }

            ImageButton {
                anchors.verticalCenter: parent.verticalCenter
                id: playButton
                source: {
                    if (spotify.playbackState === Spotify.PlayState)
                        return "pause.png"
                    else if (spotify.playbackState === Spotify.PausedState)
                        return "play.png"

                    return "play.png"
                }
                focusSource: {
                    if (spotify.playbackState === Spotify.PlayState)
                        return "pause.png"
                    else if (spotify.playbackState === Spotify.PausedState)
                        return "play.png"

                    return "play.png"
                }

                onClicked: {
                    if (spotify.playbackState === Spotify.PlayState)
                        spotify.pause()
                    else if (spotify.playbackState === Spotify.PausedState)
                        spotify.play()
                }
            }

            ImageButton {
                anchors.verticalCenter: parent.verticalCenter
                id: nextButton
                source: "next.png"
                focusSource: "next.png"

                onClicked: sonata.next()
            }
        }

        Column {
            anchors.left: playbackControl.right
            anchors.right: parent.right
            anchors.verticalCenter: playbackControl.verticalCenter

            Text {
                text: "ARTIST"
                color: Qt.darker(artistName.color, 1.5)
                font.pointSize: 15
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.leftMargin: 20
                font.bold: true
            }

            Text {
                id: artistName
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 20
                text: track.artistNames.join(", ")
                color: "#efefef"
                font.pointSize: 22
                font.family: "Roboto"
                font.bold: true
                elide: Text.ElideRight
            }
        }
    }

    Item {
        anchors.top: trackInfo.bottom
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.right: parent.right
        property color foreground: "#afafaf"

        height: 25

        Text {
            id: positionLabel
            color: parent.foreground
            text: Time.formatTime(spotify.position)
            anchors.verticalCenter: trackBar.verticalCenter
            anchors.left: parent.left
            width: 40
            horizontalAlignment: Text.AlignRight
            font.family: "Roboto"
            font.pointSize: 14
        }

        Trackbar {
            id: trackBar
            anchors.left: positionLabel.right
            anchors.right: durationLabel.left
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.leftMargin: 10
            anchors.rightMargin: 10

            backgroundColor: "#88000000"
            foregroundColor: "#afafaf"
            border.color: "#55afafaf"
            border.width: 1

            position: track.isValid ? spotify.position / track.duration : 0

            onClicked: spotify.seek(pos * track.duration)
        }

        Text {
            anchors.right: parent.right
            anchors.verticalCenter: trackBar.verticalCenter
            width: 40
            id: durationLabel
            color: parent.foreground
            text: Time.formatTime(track.duration)
            font.family: "Roboto"
            font.pointSize: 14
        }
    }
}
