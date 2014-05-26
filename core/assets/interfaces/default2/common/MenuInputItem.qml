import QtQuick 2.3
import Navigation 0.1

import "."

MenuItem {
    id: root

    property alias labelWidth: labelItem.width
    property alias label: labelItem.text
    property alias input: inputItem.text
    property alias inputHeader: keyboard.header

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
        color: UI.colors.label
    }

    Text {
        id: inputItem
        y: UI.menu.verticalMargins
        anchors {
            left: labelItem.right
            right: root.right
            margins: UI.menu.inputSpacing
        }

        font: UI.fonts.standard
        color: root.activeFocus ? UI.colors.focusText : UI.colors.text

        Behavior on color {
            ColorAnimation { duration: UI.timing.highlightMove }
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
        console.log("Setting text to " + root.input)
        keyboard.activate()
    }
}
