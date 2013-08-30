import QtQuick 2.0

FocusScope {
    width: 100
    height: 60

    property alias placeholder: placeholder.text
    property alias text: input.text
    property alias displayText: input.displayText
    property alias echoMode: input.echoMode

    property alias color: input.color
    property alias font: input.font
    property alias verticalAlignment: input.verticalAlignment
    property alias horizontalAlignment: input.horizontalAlignment

    // Use this property to access the other properties of the TextInput
    readonly property alias input: input

    Text {
        id: placeholder
        color: input.color
        font: input.font
        verticalAlignment: input.verticalAlignment
        horizontalAlignment: input.horizontalAlignment
        anchors.fill: parent

        opacity: input.text == "" ? 0.5 : 0
        Behavior on opacity {
            NumberAnimation { }
        }
    }

    TextInput {
        id: input
        focus: true
        anchors.fill: parent
        passwordCharacter: "*"
    }

    function append(s)
    {
        input.insert(input.cursorPosition, s)
    }

    function backspace()
    {
        if (input.cursorPosition > 0)
            input.remove(input.cursorPosition - 1, input.cursorPosition)
    }
}
