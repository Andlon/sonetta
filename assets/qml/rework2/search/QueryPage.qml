import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

FocusScope
{
    Label {
        text: "QUERY"
        anchors {
            bottom: keyboard.top
            left: keyboard.left
            margins: ui.misc.globalPadding
        }
    }

    VirtualKeyboardInput {
        id: keyboard
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
            margins: ui.misc.globalPadding
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
    }

    Label {
        text: "SUGGESTIONS"
        anchors {
            bottom: predictions.top
            left: predictions.left
            margins: ui.misc.globalPadding
        }

        opacity: predictions.count > 0 ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: ui.misc.globalAnimationTime }
        }
    }

    MenuView {
        id: predictions
        model: search.predictions
        anchors {
            left: keyboard.right
            right: parent.right
            bottom: parent.bottom
            top: keyboard.top
            margins: ui.misc.globalPadding
            topMargin: 0
        }

        onItemPressed: {
            keyboard.setText(data.modelData)
            keyboard.focus = true
        }

        Navigation.onLeft: keyboard.focus = true
    }
}
