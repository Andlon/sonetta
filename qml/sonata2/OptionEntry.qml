import QtQuick 2.0
import com.bitfraction.sonata 0.1

Rectangle {
    id: root
    property alias label: text.text
    property alias textColor: text.color
    property alias font: text.font
    signal clicked

    height: 40

    color: activeFocus ? "#30ffffff" : "transparent"

    anchors {
        left: parent.left
        right: parent.right
    }

    Text {
        id: text
        font.family: "Roboto"
        font.pointSize: 16
        color: "White"

        anchors.verticalCenter: root.verticalCenter
        anchors.margins: 30
        anchors.left: root.left
        anchors.right: root.right

        elide: Text.ElideRight
    }

    Navigation.onOk: root.clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
