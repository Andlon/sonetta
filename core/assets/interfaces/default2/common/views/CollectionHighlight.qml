import QtQuick 2.3

import ".."

Item {
    id: root
    property ListView list: null
    property double indentation: 0

    // Keep behavior no indentation rather than highlight.width
    // to avoid animation when the width is explicitly set
    Behavior on indentation {
        SmoothedAnimation { duration: UI.timing.highlightMove; velocity: -1 }
    }

    states: [
        State {
            when: list === null || !list.currentItem
            PropertyChanges { target: root; opacity: 0 }
        },
        State {
            when: list && list.currentItem && list.activeFocus
            PropertyChanges { target: root; opacity: 1 }
            PropertyChanges { target: highlight; color: UI.colors.focus }
        },
        State {
            when: list && list.currentItem && !list.activeFocus
            PropertyChanges { target: root; opacity: 1 }
            PropertyChanges { target: highlight; color: UI.colors.currentUnfocused }
        }
    ]

    transitions: Transition {
        ColorAnimation { property: "color"; duration: UI.timing.highlightMove }
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
