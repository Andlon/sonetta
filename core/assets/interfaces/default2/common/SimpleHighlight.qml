import QtQuick 2.3

import "."

Rectangle {
    id: root

    property bool selected: false
    property bool active: false

    states: [
        State {
            when: !selected
            PropertyChanges { target: root; opacity: 0; color: UI.colors.currentUnfocused }
        },
        State {
            when: selected && !active
            PropertyChanges { target: root; opacity: 1; color: UI.colors.currentUnfocused }
        },
        State {
            when: selected && active
            PropertyChanges { target: root; opacity: 1; color: UI.colors.focus }
        }
    ]

    transitions: Transition {
        ColorAnimation { property: "color"; duration: UI.timing.highlightMove }
        SmoothedAnimation { property: "opacity"; duration: UI.timing.highlightMove; velocity: -1 }
    }
}
