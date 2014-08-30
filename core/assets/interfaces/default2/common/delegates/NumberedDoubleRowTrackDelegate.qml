import QtQuick 2.3

import "../"

Item {
    id: delegateRoot
    height: col.height + 2 * col.anchors.topMargin
    focus: true

    property int number: model ? model.collectionIndex + 1 : 0
    property string track: model ? model.name : ""
    property string artist: model ? model.artistNames.join(", ") : ""

    states: [
        State {
            when: delegateRoot.activeFocus
            PropertyChanges { target: trackLabel; color: UI.colors.focusText }
            PropertyChanges { target: artistLabel; color: UI.colors.focusText }
            PropertyChanges { target: number; color: UI.colors.focusLabel }
        },
        State {
            when: !delegateRoot.activeFocus
            PropertyChanges { target: trackLabel; color: UI.colors.text }
            PropertyChanges { target: artistLabel; color: UI.colors.label }
            PropertyChanges { target: number; color: UI.colors.label }
        }
    ]

    transitions: Transition {
        ColorAnimation { duration: UI.timing.highlightMove }
    }

    Text {
        id: measurementDummy
        visible: false
        font: number.font
        text: "00"
    }

    Text {
        id: number
        anchors {
            verticalCenter: col.verticalCenter
            left: delegateRoot.left
            margins: UI.globalSpacing
        }
        visible: delegateRoot.number > 0
        font: UI.playlist.doubleRowNumberFont
        text: delegateRoot.number
        width: measurementDummy.width
        horizontalAlignment: Text.AlignRight
    }

    Column {
        id: col
        anchors {
            top: delegateRoot.top
            left: number.right
            right: delegateRoot.right
            margins: UI.globalSpacing
            topMargin: UI.globalSpacing / 2
        }

        Text {
            id: trackLabel
            text: track
            font: UI.playlist.doubleRowTrackFont
            elide: Text.ElideRight

            anchors {
                left: parent.left
                right: parent.right
            }
        }

        Text {
            id: artistLabel
            text: artist
            font: UI.playlist.doubleRowArtistFont
            elide: Text.ElideRight
            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }
}
