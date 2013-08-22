import QtQuick 2.0
import sonata 0.1

FocusScope {
    id: root
    //    height: controls.height + 40
    height: trackbar.height + trackbar.anchors.bottomMargin + controls.height + controls.anchors.bottomMargin + border.height
    width: 800

    readonly property bool trackIsValid: info.isValid

    TrackInfo {
        id: info
        track: spotify.currentTrack
    }

    Rectangle {
        id: backgroundFiller
        anchors.top: background.bottom
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: root.bottom
        color: Qt.darker(ui.colors.background, 2.2)
    }

    Rectangle {
        id: background
        anchors.left: root.left
        anchors.right: root.right
        anchors.bottom: controls.bottom
        anchors.top: controls.top
        color: ui.colors.background

        gradient:  Gradient {
            GradientStop { position: 0; color: Qt.lighter(background.color, 1.2) }
            GradientStop { position: 0.49; color: Qt.darker(background.color, 1.4); }
            GradientStop { position: 0.51; color: Qt.darker(background.color, 1.6); }
            GradientStop { position: 1; color: Qt.darker(background.color, 2.2); }
        }
    }

    Rectangle {
        id: border
        anchors.bottom: controls.top
        anchors.left: root.left
        anchors.right: root.right

        height: 3
        color: ui.colors.border
    }

    Image {
        id: albumCover
        source: "images/albumart.jpeg"
        width: 120
        height: 120
        anchors.left: root.left
        anchors.margins: 10
        anchors.verticalCenter: root.verticalCenter
    }

    PlaybackControls {
        id: controls
        anchors.left: albumCover.right
        anchors.bottom: trackbar.top
        anchors.margins: 20
        anchors.bottomMargin: 5

        focus: true
    }

    Text {
        id: trackLabel
        font.family: ui.fonts.headerFamily
        font.pointSize: ui.fonts.headerSize
        color: ui.colors.playbackText

        anchors.left: controls.right
        anchors.right: spotifyCore.left
        anchors.bottom: controls.verticalCenter
        anchors.rightMargin: 20
        anchors.leftMargin: 20

        text: info.name
    }

    Text {
        id: artistLabel
        font.family: ui.fonts.headerFamily
        font.pointSize: ui.fonts.defaultSize
        color: ui.colors.playbackText

        anchors.left: controls.right
        anchors.right: spotifyCore.left
        anchors.top: controls.verticalCenter
        anchors.rightMargin: 20
        anchors.leftMargin: 20
        horizontalAlignment: Text.AlignLeft

        text: info.artistNames.join(", ")
    }

    Image {
        id: spotifyCore
        source: "images/core.png"
        width: 120
        height: 120
        anchors.right: root.right
        anchors.margins: 10
        anchors.verticalCenter: root.verticalCenter
    }

    Trackbar {
        id: trackbar
        height: 25
        anchors.left: albumCover.right
        anchors.right: spotifyCore.left
        anchors.bottom: root.bottom
        anchors.margins: 10
        anchors.leftMargin: 20
        anchors.rightMargin: 20

        position: spotify.position
        duration: info.duration
    }
}
