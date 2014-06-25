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

    height: inputItem.height + UI.menu.verticalMargins * 2

    Text {
        id: labelItem

        anchors {
            top: inputItem.top
            bottom: inputItem.bottom
            left: root.left
            leftMargin: UI.menu.horizontalMargins
        }

        width: UI.menu.defaultLabelWidth
        font: UI.fonts.standard
        color: root.activeFocus ? UI.colors.focusText : UI.colors.text
        elide: Text.ElideRight

        Behavior on color {
            ColorAnimation { duration: UI.timing.highlightMove }
        }
    }

    Text {
        id: inputItem
        y: UI.menu.verticalMargins
        anchors {
            left: labelItem.right
            right: root.right
            margins: UI.menu.horizontalMargins
        }

        text: root.password ? createPasswordCharacters(root.input.length) : root.input
        font: UI.fonts.standard
        color: root.activeFocus ? UI.colors.focusLabel : UI.colors.label
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
