import QtQuick 2.3

import ".."

MenuItem {
    id: root
    property alias text: textElement.text

    height: textElement.height + 2 * UI.menu.verticalMargins

    FocusText {
        id: textElement
        y: UI.menu.verticalMargins
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: UI.menu.horizontalMargins + root.indentation
            rightMargin: UI.menu.horizontalMargins
        }

        font: UI.fonts.standard
        elide: Text.ElideRight
        hasFocus: root.activeFocus
    }
}
