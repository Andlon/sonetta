import QtQuick 2.3
import QtGraphicalEffects 1.0

import "../../common"

Item {
    id: root

    property bool selected: false

    property color focusedColor: UI.colors.focusText
    property color selectedColor: UI.colors.text
    property color unselectedColor: UI.colors.text
    property alias source: image.source

    states: [
        State {
            when: !root.selected
            PropertyChanges { target: overlay; color: root.unselectedColor }
        },
        State {
            when: root.selected && !root.activeFocus
            PropertyChanges { target: overlay; color: root.selectedColor }
        },
        State {
            when: root.selected && root.activeFocus
            PropertyChanges { target: overlay; color: root.focusedColor }
        }
    ]

    transitions: Transition {
        ColorAnimation { property: "color"; duration: UI.timing.highlightMove }
    }

    Image {
        id: image
        sourceSize: Qt.size(width, height)
        anchors.fill: root
        visible: false
    }

    ColorOverlay {
        id: overlay
        source: image
        anchors.fill: root
    }

}
