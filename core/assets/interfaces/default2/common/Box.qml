import QtQuick 2.2

import "."

Rectangle {
    border.color: UI.box.borderColor
    border.width: UI.box.borderWidth
    color: "transparent"

    property alias pattern: patternItem.pattern

    Pattern {
        id: patternItem
        anchors.fill: parent
        anchors.margins: parent.border.width
        pattern: UI.box.backgroundPattern
    }
}
