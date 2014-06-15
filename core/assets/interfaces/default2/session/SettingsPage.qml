import QtQuick 2.3

import "../common"

FocusScope {
    Rectangle {
        anchors.fill: parent
        anchors.margins: UI.globalSpacing
        width: 100
        height: 100

        color: "Green"
        border.color: UI.colors.light
        border.width: 1
    }
}
