import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

FocusScope
{
    Label {
        id: historyLabel
        text: "Recent Searches"
        anchors {
            top: queryLabel.top
            left: history.left
            leftMargin: ui.misc.globalPadding
        }
    }

    MenuView {
        id: history

        anchors {
            left: parent.left
            right: keyboard.left
            bottom: parent.bottom
            top: keyboard.top
            margins: ui.misc.globalPadding
        }

        focus: true
        model: ["Coldplay", "Nirvana", "AC/DC", "Map of the Problematique", "Frank Ocean",
            "Haim", "Michael Jackson", "Flashback"]

        Navigation.onRight: keyboard.focus = true
    }

    Label {
        id: queryLabel
        text: "Query"
        anchors {
            top: parent.top
            left: keyboard.left
            margins: ui.misc.globalPadding
        }
    }

    VirtualKeyboardInput {
        id: keyboard
        anchors {
            top: queryLabel.bottom
            margins: ui.misc.globalPadding
            horizontalCenter: parent.horizontalCenter
        }

        focus: true

        wrapNavigationLeft: false
        wrapNavigationRight: false

        onComplete: {
            search.go(text)
            var state = ui.state
            state.search.stage = "results"
            ui.pushState(state)
        }

        onTextChanged: { search.predict(text) }

        Navigation.onRight: predictions.focus = true
        Navigation.onLeft: history.focus = true
    }

    Label {
        text: "Suggestions"
        anchors {
            top: queryLabel.top
            left: predictions.left
            leftMargin: ui.misc.globalPadding
        }

        opacity: predictions.count > 0 ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: ui.misc.globalAnimationTime }
        }
    }

    MenuView {
        id: predictions

        anchors {
            bottom: parent.bottom
            left: keyboard.right
            top: keyboard.top
            right: parent.right
            margins: ui.misc.globalPadding
        }

        model: search.predictions

        onItemPressed: {
            keyboard.setText(currentItem.internalModelData)
            keyboard.focus = true
        }

        Navigation.onLeft: keyboard.focus = true
    }




}
