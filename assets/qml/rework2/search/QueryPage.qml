import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

FocusScope
{
    VirtualKeyboardInput {
        anchors.centerIn: parent
        focus: true

        onComplete: {
            search.go(text)
            var state = ui.state
            state.search.stage = "results"
            ui.pushState(state)
        }
    }
}
