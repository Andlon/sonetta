import QtQuick 2.3
import Sonetta 0.1
import Navigation 0.1

import "../common"
import "playback"

FocusScope {
    id: root

    readonly property int leftColumnWidth: 650

    TrackInfo {
        id: track
        track: player.track
    }

    Text {
        id: header
        font: UI.fonts.mainMenu
        text: "Album Art"
        color: UI.colors.label
        elide: Text.ElideRight
        anchors {
            top: root.top
            left: root.left
            right: coverContainer.right
            margins: UI.globalSpacing
        }
    }

    Box {
        id: coverContainer
        anchors {
            top: header.bottom
            left: controls.left
            topMargin: UI.globalSpacing / 2
        }

        height: controlsLabel.y - 2.5 * UI.globalSpacing - header.height
        width: height

        SpotifyImage {
            id: cover
            anchors {
                fill: parent
                margins: UI.globalSpacing
            }

            fillMode: Image.PreserveAspectFit
            uri: track.largeCoverUri
        }
    }

    Text {
        id: controlsLabel
        font: UI.fonts.mainMenu
        text: "Controls"
        color: UI.colors.label
        elide: Text.ElideRight
        anchors {
            bottom: controls.top
            left: root.left
            right: controls.right
            margins: UI.globalSpacing
            bottomMargin: UI.globalSpacing / 2
        }
    }

    PlaybackControls {
        id: controls
        focus: true
        anchors {
            bottom: root.bottom
            left: root.left
            right: coverContainer.right
            margins: UI.globalSpacing
            rightMargin: 0
        }

        KeyNavigation.right: rightColumn
    }

    RightColumn {
        id: rightColumn
        focus: true
        anchors {
            top: root.top
            right: root.right
            bottom: root.bottom
            left: coverContainer.right
            margins: UI.globalSpacing
        }

        KeyNavigation.left: controls
    }
}
