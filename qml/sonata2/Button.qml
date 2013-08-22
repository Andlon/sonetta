import QtQuick 2.0
import com.bitfraction.sonata 0.1

Rectangle {
    color: mouseArea.pressed ? "#adadad" : "#717171"
    width: 60
    height: 25
    smooth: true
    border.color: "Black"
    border.width: 1

    radius: 5

    signal clicked

    property alias text: label.text
    property alias textColor: label.color

    Text {
        id: label
        anchors.centerIn: parent
        anchors.margins: parent.radius

        color: "#f3f3f3"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: parent.clicked();
    }

    Keys.onSpacePressed: clicked();

    Navigation.onOk: clicked();
}
