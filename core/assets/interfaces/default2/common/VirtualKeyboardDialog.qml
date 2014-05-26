import QtQuick 2.2

import "."

VirtualDialog {
    id: root

    property string header
    property string text
    property bool password: false
    property string passwordCharacter: "*"

    Item {
        Text {
            id: header
            anchors {
                bottom: inputContainer.top
                bottomMargin: UI.globalSpacing
                horizontalCenter: inputContainer.horizontalCenter
            }

            width: contentWidth
            height: contentHeight
            font: UI.fonts.standard
            color: UI.colors.label
            text: root.header
        }

        Box {
            id: inputContainer
            anchors {
                left: keyboard.left
                right: keyboard.right
                bottom: keyboard.top
                bottomMargin: UI.globalSpacing / 2
            }

            pattern: "medium"
            height: input.height + UI.globalSpacing

            TextInput {
                id: input

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: UI.globalSpacing
                }

                y: UI.globalSpacing / 2
                width: keyboard.width
                clip: true
                font: UI.fonts.input
                color: UI.colors.text
                cursorVisible: true
                horizontalAlignment: TextInput.AlignHCenter
                echoMode: root.password ? TextInput.Password : TextInput.Normal
                passwordCharacter: root.passwordCharacter

                onTextChanged: root.text = text

                Connections {
                    target: root
                    onActivating: input.text = root.text
                }
            }
        }

        VirtualKeyboard {
            id: keyboard
            anchors.centerIn: parent
            focus: true

            onDone: root.deactivate()
            onCharacter: input.insert(input.cursorPosition, character)
            onSpace: input.insert(input.cursorPosition, " ")
            onBack: if (input.cursorPosition > 0) input.remove(input.cursorPosition - 1, input.cursorPosition)
        }
    }
}
