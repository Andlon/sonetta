import QtQuick 2.2
import Sonetta 0.1
import Navigation 0.1

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

    property bool active: false
    property bool collapsible: false
    property bool allowDownToActive: true

    signal complete

    states: [
        State {
            name: "active"
            when: active
            PropertyChanges {
                target: keyboard
                opacity: 1.0
                focus: true
            }
        },
        State {
            name: "inactive"
            when: !active && !collapsible
            PropertyChanges {
                target: keyboard
                opacity: 0.3
                focus: false
            }
        },
        State {
            name: "collapsed"
            extend: "inactive"
            when: !active && collapsible
            PropertyChanges {
                target: keyboard
                height: 0
                restoreEntryValues: true
            }
        }
    ]

    transitions: Transition {
        SmoothedAnimation { properties: "opacity, height"; duration: ui.misc.globalAnimationTime; velocity: -1 }
    }

    onActiveFocusChanged: {
        if (!activeFocus)
        {
            // Lost focus, set to inactive/collapsed
            active = false
        }
    }

    Column {
        width: childrenRect.width
        height: childrenRect.height
        spacing: ui.misc.globalPadding / 2
        clip: true

        Item {
            states: [
                State {
                    name: "hint"
                    when: root.activeFocus && !root.active && root.text === ""
                    PropertyChanges { target: input; opacity: 0; }
                    PropertyChanges { target: hintLabel; opacity: 0.8 }
                },
                State {
                    name: "nohint"
                    when: !root.activeFocus || root.active || root.text !== ""
                    PropertyChanges { target: input; opacity: 1; }
                    PropertyChanges { target: hintLabel; opacity: 0 }
                }
            ]

            transitions: Transition {
                OpacityAnimator {
                    easing.type: Easing.InOutQuad
                    duration: 2 * ui.misc.globalAnimationTime
                }
            }

            width: keyboard.width
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

            Text {
                id: hintLabel
                anchors.fill: input
                font: input.font
                color: ui.colors.label

                text: "Press to type..."
            }
        }

        VirtualKeyboard {
            id: keyboard

            focus: true

            onCharacter: root.insert(c)
            onBackspace: root.backspace(autorepeat)
            onEnter: {
                active = false;
                root.complete()
            }

            highlightColor: ui.colors.highlight

            Keys.forwardTo: Nav {
                onUp: root.active = false
            }
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

    function backspace(autorepeat)
    {
        if (input.cursorPosition > 0)
            input.remove(input.cursorPosition - 1, input.cursorPosition)
        else if (!autorepeat)
            root.active = false;
    }

    function clear()
    {
        input.text = ""
    }

    Keys.onPressed: {
        // Match any "word" character
        var validChars = /\w+/
        if (!active && validChars.test(event.text))
        {
            insert(event.text)
            active = true
            event.accepted = true
        }
    }

    Keys.forwardTo: Nav {
        onOk: {
            if (!active && !event.isAutoRepeat)
                active = true
        }

        onDown: {
            if (!active && !event.isAutoRepeat && allowDownToActive)
                active = true
            else
                event.accepted = false
        }
    }
}
