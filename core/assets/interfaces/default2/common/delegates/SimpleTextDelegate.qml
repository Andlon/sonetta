import QtQuick 2.3

import QtQuick 2.3

import "../."
import "../Time.js" as Time

Item {
    id: root
    height: label.height + UI.globalSpacing
    focus: true

    property alias text: label.text

    states: [
        State {
            when: root.activeFocus
            PropertyChanges { target: label; color: UI.colors.focusText }
        },
        State {
            when: !root.activeFocus
            PropertyChanges { target: label; color: UI.colors.text }
        }
    ]

    transitions: Transition {
        ColorAnimation { duration: UI.timing.highlightMove }
    }

    Text {
        id: label
        anchors {
            left: parent.left
            top: parent.top
            margins: UI.globalSpacing
            topMargin: UI.globalSpacing / 2
        }

        font: UI.fonts.standard
        elide: Text.ElideRight
        color: UI.colors.text
    }
}

