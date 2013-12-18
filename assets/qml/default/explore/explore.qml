import QtQuick 2.2
import Sonetta 0.1
import "../common"

Page {
    page: "explore"

    StackView {
        id: stack
        focus: true
        anchors {
            topMargin: ui.misc.topSectionHeight
            fill: parent
        }
    }

    Connections {
        target: UI

        onParametersChanged: {
            if (UI.page === page)
            {
                if (UI.parameters.type === "album")
                {
                    var properties = { album: UI.parameters.album }
                    stack.pop()
                    stack.push(Qt.resolvedUrl("AlbumBrowse.qml"), properties)
                }
            }
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
