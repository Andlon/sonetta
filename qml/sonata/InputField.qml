import QtQuick 2.0

FocusScope {
    id: root

    antialiasing: true
    width: 300
    height: 65

    property alias echoMode: input.echoMode
    property alias label: label.text
    property alias text: input.text

    Rectangle {
        // Background
        color: ui.colors.inputBackground
        border.color: root.activeFocus ? ui.colors.text : ui.colors.border
        border.width: 2
        anchors.fill: parent
        radius: 15
    }

    Text {
        id: label
        color: ui.colors.text
        anchors.verticalCenter: root.verticalCenter
        anchors.left: root.left
        anchors.margins: { left: 25; right: 25; top: 10; bottom: 10; }

        font.family: ui.fonts.defaultFamily
        font.pointSize: ui.fonts.defaultSize

        width: 130

        elide: Text.ElideRight
    }

    TextInput {
        id: input
        color: ui.colors.inputText
        clip: true

        font.family: ui.fonts.defaultFamily
        font.pointSize: ui.fonts.defaultSize

        anchors.verticalCenter: root.verticalCenter
        anchors.left: label.right
        anchors.right: root.right
        anchors.margins: { left: 10; right: 10; top: 5; bottom: 5; }

        focus: true
    }
}
