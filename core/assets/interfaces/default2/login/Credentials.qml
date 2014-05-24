import QtQuick 2.2
import QtQml.Models 2.1
import Navigation 0.1

import "../common"

ListView {
    id: root
    height: contentItem.childrenRect.height
    width: contentItem.childrenRect.width

    highlightMoveDuration: UI.timing.highlightMove

    currentIndex: 0

    highlight: Rectangle {
        color: UI.colors.focus
    }

    model: ObjectModel {
        MenuTextItem {
            text: "Username"

            VirtualKeyboardDialog {
                id: keyboard

                header: "Enter your Spotify™ username"
                onDeactivated: root.forceActiveFocus()
            }

            Navigation.onOk: keyboard.activate()
        }

        MenuTextItem {
            text: "Password"
        }

        MenuTextItem {
            text: "Remember me"
        }

        MenuTextItem {
            text: "Log in"
        }

        MenuTextItem {
            text: "Exit"

            Navigation.onOk: Qt.quit()
        }
    }

    Navigation.onDown: root.incrementCurrentIndex()
    Navigation.onUp: root.decrementCurrentIndex()
}
