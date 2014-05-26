import QtQuick 2.3
import Navigation 0.1
import Sonetta 0.1

import "."

MenuItem {
    id: root
    property alias text: textElement.text
    property bool checked: false

    height: textElement.height + 2 * UI.menu.verticalMargins

    Text {
        id: textElement
        y: UI.menu.verticalMargins
        anchors {
            left: parent.left
            right: frame.left
            leftMargin: UI.menu.horizontalMargins
            rightMargin: UI.menu.horizontalMargins
        }

        font: UI.fonts.standard
        elide: Text.ElideRight

        states: [
            State {
                when: root.activeFocus
                PropertyChanges { target: textElement; color: UI.colors.focusText }
                PropertyChanges { target: frame; border.color: UI.colors.focusLabel }
                PropertyChanges { target: indicator; color: UI.colors.focusLabel }
            },
            State {
                when: !root.activeFocus
                PropertyChanges { target: textElement; color: UI.colors.text }
                PropertyChanges { target: frame; border.color: UI.colors.label }
                PropertyChanges { target: indicator; color: UI.colors.label }
            }
        ]

        transitions: Transition {
            ColorAnimation {
                duration: UI.timing.highlightMove
            }
        }
    }

    Rectangle {
        id: frame
        anchors {
            top: textElement.top
            right: root.right
            bottom: textElement.bottom
            rightMargin: UI.menu.horizontalMargins
            topMargin: UI.menu.checkboxMargin
            bottomMargin: UI.menu.checkboxMargin
        }

        border.width: UI.menu.frameSpacing
        color: "transparent"
        rotation: 45

        width: height

        Rectangle {
            id: indicator
            anchors {
                fill: parent
                margins: 2 * frame.border.width
            }

            visible: root.checked
        }
    }

    Navigation.onOk: root.checked = !root.checked
}
