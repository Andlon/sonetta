import QtQuick 2.2
import QtQml.Models 2.1
import Sonetta 0.1
import "../common"

FocusScope {
    focus: true

    PageView {
        id: root
        // Apparently this is necessary, no idea why
        focus: true

        anchors.fill: parent
        anchors.topMargin: ui.misc.topSectionHeight

        currentIndex: 0

        model: ObjectModel {
            QueryPage {
                width: root.cellWidth
                height: root.cellHeight
            }

            ResultsPage {
                width: root.cellWidth
                height: root.cellHeight
            }
        }

        Navigation.onBack: {
            var state = ui.state
            if (state.search.stage === "results")
            {
                state.search.stage = "query"
                ui.updateState(state)
            }
        }
    }

    Connections {
        target: ui

        onStatePushed: updateCurrentIndex()
        onStatePopped: updateCurrentIndex()
        onStateReset: {
            if (ui.state.page === "search" && ui.state.search.stage === "query")
            {
                root.highlightFollowsCurrentItem = false
                updateCurrentIndex()
                root.highlightFollowsCurrentItem = true
            }
        }
    }

    function updateCurrentIndex()
    {
        var state = ui.state
        if (state.search.stage === "query")
            root.currentIndex = 0
        else if (state.search.stage === "results")
            root.currentIndex = 1
    }
}
