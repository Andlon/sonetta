import QtQuick 2.3
import Sonetta 0.1
import "../../common"

Item {
    id: root
    height: labels.height + UI.globalSpacing

    property alias selected: highlight.selected
    readonly property font font: UI.fonts.standard

    states: [
        State {
            when: !root.selected && (root.activeFocus || !root.activeFocus)
            PropertyChanges { target: labels; color: UI.colors.darkLabel }
            PropertyChanges { target: info; color: UI.colors.text }
        },
        State {
            when: root.selected && !root.activeFocus
            PropertyChanges { target: labels; color: UI.colors.darkFocusLabel }
            PropertyChanges { target: info; color: UI.colors.text }
        },
        State {
            when: root.selected && root.activeFocus
            PropertyChanges { target: labels; color: UI.colors.focusLabel }
            PropertyChanges { target: info; color: UI.colors.focusText }
        }
    ]

    transitions: Transition {
        ColorAnimation { duration: UI.timing.highlightMove; }
    }

    TrackInfo {
        id: track
        track: player.track
    }

    Box {
        anchors.fill: root
    }

    SimpleHighlight {
        id: highlight
        anchors.fill: root
        active: root.activeFocus
    }

    Column {
        id: labels
        height: childrenRect.height
        width: childrenRect.width
        spacing: UI.globalSpacing / 2
        anchors {
            left: root.left
            top: root.top
            margins: UI.globalSpacing
            topMargin: UI.globalSpacing / 2
        }

        property color color

        Text {
            id: trackNameLabel
            font: root.font
            color: labels.color
            text: "TRACK"
            width: paintedWidth
        }

        Text {
            id: artistNameLabel
            font: root.font
            color: labels.color
            text: "ARTIST"
            width: paintedWidth
        }

        Text {
            id: albumNameLabel
            font: root.font
            color: labels.color
            text: "ALBUM"
            width: paintedWidth
        }
    }

    Column {
        id: info
        height: childrenRect.height
        spacing: UI.globalSpacing / 2
        anchors {
            left: labels.right
            right: root.right
            top: root.top
            margins: UI.globalSpacing
            topMargin: UI.globalSpacing / 2
        }

        property color color

        Text {
            id: trackName
            font: root.font
            color: info.color
            text: track.name
            anchors {
                left: parent.left
                right: parent.right
            }
        }

        Text {
            id: artistName
            font: root.font
            color: info.color
            text: track.artistNames.join(", ")
            anchors {
                left: parent.left
                right: parent.right
            }
        }

        Text {
            id: albumName
            font: root.font
            color: info.color
            text: track.albumName
            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }
}
