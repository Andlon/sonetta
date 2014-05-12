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
        }
    }

    Keys.forwardTo: Nav {
        onDown: root.incrementCurrentIndex()
        onUp: root.decrementCurrentIndex()
    }
}
