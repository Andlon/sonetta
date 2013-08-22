import QtQuick 2.0
import com.bitfraction.sonata 0.1

Rectangle {
    id: root
    height: 45
    width: 300

    property color primaryColor: "#44000044"
    property color focusColor: Qt.tint(primaryColor, "#660096ff")

    border.width: 2
    border.color: activeFocus || keyboard.activeFocus  ? "#77ff9c00" : Qt.lighter(primaryColor, 2.0)

    property alias label: label.text

    property alias text: input.text
    property alias echoMode: input.echoMode
    property alias textColor: input.color

    property alias font: input.font
    property alias labelFont: label.font
    property alias labelColor: label.color

    property int labelWidth: label.width

    color: activeFocus || keyboard.activeFocus ? focusColor : primaryColor

    signal keyboardClosed

    Row {
        anchors.fill: parent
        anchors.margins: 5
        clip: true
        spacing: 5

        Text {
            id: label
            font: input.font
            width: 120
            color: input.color

            anchors.verticalCenter: parent.verticalCenter
            elide: Text.ElideRight
        }

        TextInput {
            id: input

            font.family: "Roboto"
            font.pointSize: 16
            anchors.verticalCenter: parent.verticalCenter
            readOnly: true
            color: "White"
        }
    }

    VirtualKeyboard {
        id: keyboard

        onInputTextChanged: input.text = inputText

        onClosed: root.keyboardClosed();

        isPassword: root.echoMode == TextInput.Password
    }

    MouseArea {
        anchors.fill: parent

        onClicked: keyboard.show(root.text)
    }

    Navigation.onOk: if (activeFocus) keyboard.show(root.text)
}
