import QtQuick 2.1

FocusScope {
    id: root

    width: childrenRect.width
    height: childrenRect.height

    property int barWidth: 8

    readonly property string text: input.text

    signal complete

    Column {
        width: childrenRect.width
        height: childrenRect.height
        spacing: ui.misc.globalPadding / 2

        Item {
            width: keyboard.width - 2 * ui.misc.globalPadding
            x: ui.misc.globalPadding
            height: childrenRect.height


            Rectangle {
                id: rightBar
                anchors {
                    right: parent.right
                    bottom: underline.top
                }

                color: ui.colors.highlight
                width: barWidth
                height: barWidth
            }

            Rectangle {
                id: leftBar
                anchors {
                    left: parent.left
                    bottom: underline.top
                }

                color: ui.colors.highlight
                width: barWidth
                height: barWidth
            }

            Rectangle {
                id: underline
                width: keyboard.width
                height: barWidth
                color: ui.colors.highlight

                anchors {
                    top: input.bottom
                    left: leftBar.left
                    right: rightBar.right
                    topMargin: barWidth
                }
            }

            TextInput {
                id: input
                enabled: false
                clip: true
                cursorVisible: keyboard.activeFocus

                anchors {
                    left: leftBar.right
                    right: rightBar.left
                    margins: barWidth
                }

                height: contentHeight

                font: ui.fonts.h2
                color: ui.colors.standard
            }
        }

        VirtualKeyboard {
            id: keyboard

            focus: true

            onCharacter: root.insert(c)
            onBackspace: root.backspace()
            onEnter: root.complete()

            wrapNavigationLeft: false

            highlightColor: ui.colors.highlight
        }
    }

    function insert(text)
    {
        input.insert(input.cursorPosition, text)
    }

    function backspace()
    {
        if (input.cursorPosition > 0)
            input.remove(input.cursorPosition - 1, input.cursorPosition)
    }
}
