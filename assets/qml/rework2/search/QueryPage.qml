import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

FocusScope
{
    VirtualKeyboardInput {
        id: keyboard
        anchors.centerIn: parent
        focus: true

        onComplete: {
            search.go(text)
            var state = ui.state
            state.search.stage = "results"
            ui.pushState(state)
        }

        onTextChanged: { search.predict(text) }
    }

    CollectionView {
        id: predictions
        model: search.predictions
        anchors {
            left: keyboard.right
            right: parent.right
            bottom: parent.bottom
            top: keyboard.top
            margins: ui.misc.globalPadding
        }

        delegate: Text {
            text: modelData
            width: predictions.width
            height: 50
            font: ui.fonts.h4
            color: ui.colors.standard
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }
}
