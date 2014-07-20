import QtQuick 2.3

import ".."

MenuTextItem {
    id: root
    indentation: menu && menu.indentation ? menu.indentation : UI.menu.indentation

    property bool lastChild: false

    Item {
        anchors {
            left: root.left
            top: root.top
            bottom: root.bottom
        }

        width: root.indentation

        // Make sure that highlight displays on top of child indicator
        z: -1
        parent: root.parent

        Rectangle {
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: root.lastChild ? parent.verticalCenter : parent.bottom
            }
            width: 2
            color: root.border.color
        }

        Rectangle {
            anchors {
                verticalCenter: parent.verticalCenter
                left: parent.horizontalCenter
                right: parent.right
            }
            height: 2
            color: root.border.color
        }
    }
}
