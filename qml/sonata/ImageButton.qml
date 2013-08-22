import QtQuick 2.0
import sonata 0.1

Item {
    id: root
    property string source
    property string focusSource

    property alias label: label.text
    property alias labelFont: label.font
    property alias labelColor: label.color

    width: img.sourceSize.width
    height: img.sourceSize.height

    signal clicked

    Keys.onReturnPressed: root.clicked()
    Navigation.onOk: root.clicked()

    Image {
        id: img
        anchors.fill: root
        smooth: true
        width: root.width
        height: root.height
        source: root.activeFocus || area.pressed ? root.focusSource : root.source;
    }

    Text {
        id: label
        anchors.centerIn: root

        font.family: "Roboto"
        font.pointSize: 16
        color: "White"

        visible: text != ""
    }

    MouseArea {
        id: area
        anchors.fill: root

        onClicked: root.clicked()
    }
}

