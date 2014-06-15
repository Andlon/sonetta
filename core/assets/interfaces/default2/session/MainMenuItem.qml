import QtQuick 2.3

import "../common"

MenuItem {
    id: root
    height: 165
    visibleBox: false

    property alias label: labelItem.text

    Text {
        id: labelItem
        anchors.centerIn: root
        font: UI.fonts.mainMenu

        states: [
            State {
                when: root.activeFocus
                PropertyChanges { target: labelItem; color: UI.colors.focusText }
            },
            State {
                when: !root.activeFocus
                PropertyChanges { target: labelItem; color: UI.colors.label }
            }
        ]

        transitions: Transition {
            ColorAnimation { duration: UI.timing.highlightMove }
        }
    }

    Rectangle {
        anchors {
            bottom: root.bottom
            left: root.left
            right: root.right
        }

        height: 1
        color: UI.colors.light

        // See MenuItem for the rationale behind the following properties
        z: -1
        parent: root.parent
    }
}
