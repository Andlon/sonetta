import QtQuick 2.3

import "../."
import "../Time.js" as Time

Item {
    id: root
    height: trackLabel.height + UI.globalSpacing
    focus: true

    property string track: model ? model.name : ""
    property string artist: model ? model.artistNames.join(", ") : ""
    property int duration: model ? model.duration : 0

    states: [
        State {
            when: root.activeFocus
            PropertyChanges { target: trackLabel; color: UI.colors.focusText }
            PropertyChanges { target: durationLabel; color: UI.colors.focusLabel }
            PropertyChanges { target: artistLabel; color: UI.colors.focusText }
        },
        State {
            when: !root.activeFocus
            PropertyChanges { target: trackLabel; color: UI.colors.text }
            PropertyChanges { target: durationLabel; color: UI.colors.label }
            PropertyChanges { target: artistLabel; color: UI.colors.text }
        }
    ]

    transitions: Transition {
        ColorAnimation { duration: UI.timing.highlightMove }
    }

    Text {
        id: trackLabel
        text: track
        font: UI.fonts.standard
        elide: Text.ElideRight
        width: root.width * 0.4
        color: UI.colors.text

        anchors {
            left: parent.left
            top: parent.top
            margins: UI.globalSpacing
            topMargin: UI.globalSpacing / 2
        }
    }


    Text {
        id: durationLabel
        text: Time.formatTime(duration)
        width: 60
        font: UI.fonts.standard
        color: UI.colors.label
        anchors {
            left: trackLabel.right
            top: trackLabel.top
            leftMargin: UI.globalSpacing
        }
    }

    Text {
        id: artistLabel
        text: artist
        font: UI.fonts.standard
        elide: Text.ElideRight
        color: UI.colors.text
        anchors {
            left: durationLabel.right
            top: durationLabel.top
            right: root.right
            leftMargin: UI.globalSpacing
            rightMargin: UI.globalSpacing
        }
    }
}
