import QtQuick 2.2

import "."

Box {
    property alias text: textElement.text
    width: UI.menuDefaultWidth
    height: textElement.height + 2 * UI.menuVerticalMargins

    Text {
        id: textElement
        y: UI.menuVerticalMargins
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: UI.menuHorizontalMargins
            rightMargin: UI.menuHorizontalMargins
        }

        font: UI.fonts.standard
        color: UI.colors.text
    }
}
