import QtQuick 2.2
import Sonetta 0.1
import "../common"

FocusScope {
    focus: true

    StackView {
        id: stack
        focus: true
        anchors {
            topMargin: ui.misc.topSectionHeight
            fill: parent
        }
    }

    Connections {
        target: ui

        onStatePushed: {
            var state = ui.state

            if (state.page === "explore")
            {
                if (state.explore.type === "album")
                {
                    var properties = {}
                    properties.album = state.explore.album

                    stack.push(Qt.resolvedUrl("AlbumBrowse.qml"), properties)
                }
            }
        }

        onStatePopped: stack.pop()
        onStateReset: stack.clear()
    }
}
