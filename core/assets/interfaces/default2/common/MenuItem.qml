import QtQuick 2.2

import "."

Box {
    property alias text: textElement.text
    width: UI.menu.defaultWidth
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
        color: UI.colors.text
    }
}
