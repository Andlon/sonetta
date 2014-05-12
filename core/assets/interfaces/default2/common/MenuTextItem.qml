import QtQuick 2.2

import "."

MenuItem {
    id: root
    property alias text: textElement.text

    height: textElement.height + 2 * UI.menu.verticalMargins

    Text {
        id: textElement
        y: UI.menu.verticalMargins
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: UI.menu.horizontalMargins
            rightMargin: UI.menu.horizontalMargins
        }

        font: UI.fonts.standard
        elide: Text.ElideRight

        states: [
            State {
                when: root.activeFocus
                PropertyChanges { target: textElement; color: UI.colors.focusText }
            },
            State {
                when: !root.activeFocus
                PropertyChanges { target: textElement; color: UI.colors.text }
            }
        ]

        transitions: Transition {
            ColorAnimation {
                duration: UI.timing.highlightMove
            }
        }
    }
}
