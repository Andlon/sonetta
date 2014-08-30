import QtQuick 2.3

import "."

Text {
    id: root
    elide: Text.ElideRight
    property bool hasFocus: false

    states: [
        State {
            when: hasFocus
            PropertyChanges { target: root; color: UI.colors.focusText }
        },
        State {
            when: !hasFocus
            PropertyChanges { target: root; color: UI.colors.text }
        }
    ]

    transitions: Transition {
        ColorAnimation { duration: UI.timing.highlightMove }
    }
}
