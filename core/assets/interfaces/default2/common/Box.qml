import QtQuick 2.2

import "."

Rectangle {
    border.color: UI.boxBorderColor
    border.width: UI.boxBorderWidth
    color: "transparent"

    Pattern {
        anchors.fill: parent
        anchors.margins: parent.border.width
        pattern: UI.boxBackgroundPattern
    }
}
