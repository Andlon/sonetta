import QtQuick 2.3

import ".."

Item {
    id: root
    property ListView list: null
    property double indentation: 0

    Behavior on indentation {
        SmoothedAnimation { duration: UI.timing.highlightMove }
    }

    states: [
        State {
            when: list === null || !list.currentItem
            PropertyChanges { target: root; opacity: 0 }
        },
        State {
            when: list && list.currentItem && list.activeFocus
            PropertyChanges { target: root; opacity: 1; width: list.currentItem.width; height: list.currentItem.height }
            PropertyChanges { target: highlight; color: UI.colors.focus }
        },
        State {
            when: list && list.currentItem && !list.activeFocus
            PropertyChanges { target: root; opacity: 1; width: list.currentItem.width; height: list.currentItem.height }
            PropertyChanges { target: highlight; color: UI.colors.currentUnfocused }
        }
    ]

    transitions: Transition {
        ColorAnimation { duration: UI.timing.highlightMove }
    }

    Rectangle {
        id: highlight
        anchors {
            right: root.right
            top: root.top
            bottom: root.bottom
        }

        width: root.width - root.indentation
    }
}
