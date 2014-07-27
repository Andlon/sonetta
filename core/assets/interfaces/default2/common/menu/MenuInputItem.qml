import QtQuick 2.3
import Navigation 0.1

import ".."

MenuItem {
    id: root

    property alias labelWidth: labelItem.width
    property alias label: labelItem.text
    property string input
    property alias inputHeader: keyboard.header
    property alias password: keyboard.password

    height: childrenRect.height + 2 * UI.menu.verticalMargins

    Text {
        id: labelItem

        anchors {
            top: root.top
            left: root.left
            right: root.right
            leftMargin: UI.menu.horizontalMargins
            rightMargin: UI.menu.horizontalMargins
            topMargin: UI.menu.verticalMargins
        }

        width: UI.menu.defaultLabelWidth
        font: UI.menu.labelFont
        color: root.activeFocus ? UI.colors.focusLabel : UI.colors.label
        elide: Text.ElideRight

        Behavior on color {
            ColorAnimation { duration: UI.timing.highlightMove }
        }
    }

    Text {
        id: inputItem
        anchors {
            top: labelItem.bottom
            left: root.left
            right: root.right
            leftMargin: UI.menu.horizontalMargins
            rightMargin: UI.menu.horizontalMargins
            topMargin: UI.menu.verticalSpacing
        }

        text: root.password ? createPasswordCharacters(root.input.length) : root.input
        font: UI.fonts.standard
        color: root.activeFocus ? UI.colors.focusText : UI.colors.text
        elide: Text.ElideRight

        Behavior on color {
            ColorAnimation { duration: UI.timing.highlightMove }
        }

        function createPasswordCharacters(length)
        {
            return new Array(1 + length).join(keyboard.passwordCharacter)
        }
    }

    VirtualKeyboardDialog {
        id: keyboard
        onDeactivating: {
            root.input = keyboard.text
            root.forceActiveFocus()
        }
    }

    Navigation.onOk: {
        keyboard.text = root.input
        keyboard.activate()
    }
}
