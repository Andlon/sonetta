import QtQuick 2.1
import Sonetta 0.1

Item {
    id: root
    width: 100
    height: 50
    clip: true

    property bool selected: false
    property real indicatorWidth: ui.misc.globalPadding / 2

    states: [
        State {
            when: root.activeFocus
            AnchorChanges { target: highlight; anchors { right: root.right; left: root.left }}
        },
        State {
            when: root.selected && !root.activeFocus
            AnchorChanges { target: highlight; anchors { right: undefined; left: root.left }}
            PropertyChanges { target: highlight; width: indicatorWidth; }
        },
        State {
            when: !root.selected && !root.activeFocus
            AnchorChanges { target: highlight; anchors { left: root.left; right: undefined }}
            PropertyChanges { target: highlight; width: 0; restoreEntryValues: true }
        }
    ]

    transitions: Transition {
        enabled: ui.misc.ready
        AnchorAnimation { duration: ui.misc.globalAnimationTime }
        SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
    }

    Rectangle {
        // Relatively ugly solution for creating horizontal-only borders
        x: -border.width
        y: 0
        width: root.width + 2 * border.width
        height: root.height

        border.color: ui.colors.light
        border.width: 2
        color: "transparent"
    }

    Rectangle {
        id: highlight
        width: indicatorWidth
        height: root.height
        color: ui.colors.highlight
    }
}
