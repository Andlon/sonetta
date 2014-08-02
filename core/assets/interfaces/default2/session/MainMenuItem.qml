import QtQuick 2.3

import "../common"

MenuItem {
    id: root
    height: 165
    visibleBox: false

    property alias label: labelItem.text

    FocusText {
        id: labelItem
        anchors.centerIn: root
        font: UI.fonts.mainMenu
        hasFocus: root.activeFocus
    }

    Rectangle {
        anchors {
            bottom: root.bottom
            left: root.left
            right: root.right
        }

        opacity: root.opacity
        visible: root.visible
        height: 1
        color: UI.colors.light

        // See MenuItem for the rationale behind the following properties
        z: -1
        parent: root.parent
    }
}
