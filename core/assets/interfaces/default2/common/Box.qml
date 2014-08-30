import QtQuick 2.2

import "."

Rectangle {
    property alias pattern: patternItem.pattern
    border.color: UI.box.borderColor
    border.width: UI.box.borderWidth
    color: "transparent"

    Pattern {
        id: patternItem
        anchors.fill: parent
        anchors.margins: parent.border.width
        pattern: UI.box.backgroundPattern
    }
}
