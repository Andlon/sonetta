import QtQuick 2.1
import Sonetta 0.1

Item {
    id: root
    height: col.height + 2 * ui.misc.globalPadding
    width: 200

    property alias label: label.text
    property alias text: input.text

    Column {
        id: col
        spacing: 0
        clip: true
        y: ui.misc.globalPadding

        anchors {
            right: root.right
            left: root.left
            margins: ui.misc.globalPadding
        }

        Text {
            id: label
            font: ui.fonts.h2
            color: ui.colors.standard
            anchors.left: parent.left
            anchors.right: parent.right
            height: contentHeight
            width: contentWidth
        }

        Text {
            id: input
            font: ui.fonts.h1
            color: ui.colors.standard
            anchors.left: parent.left
            anchors.right: parent.right
            height: contentHeight
            width: contentWidth
        }
    }
}
