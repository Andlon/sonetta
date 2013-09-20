import QtQuick 2.1
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

FocusScope
{
    Text {
        font: ui.fonts.h4
        color: ui.colors.label
        text: "QUERY: "
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

    Text {
        font: ui.fonts.h4
        color: ui.colors.label
        text: "SUGGESTIONS: "
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

    CollectionView {
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

        delegate: Item {
            width: predictions.width
            height: 65
            Text {
                anchors {
                    fill: parent
                    leftMargin: ui.misc.globalPadding
                    rightMargin: ui.misc.globalPadding
                }

                text: modelData
                font: ui.fonts.h4
                color: ui.colors.standard
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 100; duration: ui.misc.globalAnimationTime }
        }

        remove: Transition {
            NumberAnimation { property: "opacity"; from: 100; to: 0; duration: ui.misc.globalAnimationTime }
        }

        onItemPressed: {
            keyboard.setText(data.modelData)
            keyboard.focus = true
        }

        Navigation.onLeft: keyboard.focus = true
    }
}
