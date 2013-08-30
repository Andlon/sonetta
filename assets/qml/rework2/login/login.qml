import QtQuick 2.1
import Sonetta 0.1
import "../common"

Item {
    id: root

    Item {
        id: leftColumn
        anchors.left: root.left
        anchors.top: root.top
        anchors.bottom: root.bottom
        anchors.right: root.horizontalCenter
    }

    Item {
        id: rightColumn
        anchors.left: root.horizontalCenter
        anchors.right: root.right
        anchors.bottom: root.bottom
        anchors.top: root.top

        VirtualKeyboard {
            anchors.centerIn: parent
        }
    }


}
