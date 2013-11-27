import QtQuick 2.1
import Sonetta 0.1

FocusScope {
    id: root

    width: childrenRect.width
    height: childrenRect.height

    property int barWidth: 8

    readonly property string text: input.text

    property alias wrapNavigationLeft: keyboard.wrapNavigationLeft
    property alias wrapNavigationRight: keyboard.wrapNavigationRight
    property alias wrapNavigationBottom: keyboard.wrapNavigationBottom
    property alias wrapNavigationTop: keyboard.wrapNavigationTop

    signal complete

    state: "inactive"

    states: [
        State {
            name: "active"
            PropertyChanges {
                target: keyboard
                opacity: 1.0
                focus: true
            }
        },
        State {
            name: "inactive"
            PropertyChanges {
                target: keyboard
                opacity: 0.3
                focus: false
            }
        }
    ]

    transitions: Transition {
        SmoothedAnimation { properties: "opacity"; duration: ui.misc.globalAnimationTime; velocity: -1 }
    }

    onActiveFocusChanged: {
        if (!activeFocus)
        {
            // Lost focus, set to inactive
            state = "inactive"
        }
    }

    Column {
        width: childrenRect.width
        height: childrenRect.height
        spacing: ui.misc.globalPadding / 2

        Item {
            width: keyboard.width //- 2 * ui.misc.globalPadding
            //x: ui.misc.globalPadding
            height: childrenRect.height

            property color barColor: root.activeFocus ? ui.colors.highlight : ui.colors.standard

            Behavior on barColor {
                ColorAnimation { duration: ui.misc.globalAnimationTime }
            }

            Rectangle {
                id: rightBar
                anchors {
                    right: parent.right
                    bottom: underline.top
                }

                color: parent.barColor
                width: barWidth
                height: barWidth
            }

            Rectangle {
                id: leftBar
                anchors {
                    left: parent.left
                    bottom: underline.top
                }

                color: parent.barColor
                width: barWidth
                height: barWidth
            }

            Rectangle {
                id: underline
                width: keyboard.width
                height: barWidth
                color: parent.barColor

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

            highlightColor: ui.colors.highlight
        }
    }

    function setText(text)
    {
        input.text = text
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

    function clear()
    {
        input.text = ""
    }

    Navigation.onOk: {
        if (state == "inactive")
        {
            state = "active"
        }
    }
}
