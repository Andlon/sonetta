import QtQuick 2.0

Rectangle {
    id: root
    antialiasing: true
    color: ui.colors.buttonBackground
    border.color: root.activeFocus || area.pressed ? ui.colors.text : ui.colors.border
    border.width: 2
    width: 200
    height: 65

    signal clicked

    gradient: Gradient {
        GradientStop { position: 0; color: Qt.lighter(root.color, 1.2) }
        GradientStop { position: 0.49; color: Qt.darker(root.color, 1.4); }
        GradientStop { position: 0.51; color: Qt.darker(root.color, 1.6); }
        GradientStop { position: 1; color: Qt.darker(root.color, 2.2); }
    }

    radius: 15

    MouseArea {
        id: area
        anchors.fill: parent
        onClicked: root.clicked()
    }

    Keys.onReturnPressed: root.clicked()
}
