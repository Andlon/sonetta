import QtQuick 2.2
import QtQml.Models 2.1
import Sonetta 0.1
import "../common"

Page {
    id: root
    page: "search"

    property bool ignoreStage: false

    states: [
        State {
            name: "query"
            PropertyChanges { target: view; currentIndex: 0 }
        },
        State {
            name: "results"
            PropertyChanges { target: view; currentIndex: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "query"
            to: "results"
            SequentialAnimation {
                PauseAnimation { duration: view.highlightMoveDuration }
                ScriptAction { script: search.go(UI.parameters.query) }
            }
        },

        Transition {
            from: "results"
            to: "query"
            SequentialAnimation {
                PauseAnimation { duration: view.highlightMoveDuration }
                ScriptAction { script: search.clear() }
            }
        }
    ]

    PageView {
        id: view
        // Apparently this is necessary, no idea why
        focus: true
        clip: true

        anchors.fill: parent
        anchors.topMargin: ui.misc.topSectionHeight

        currentIndex: 0
        vertical: false

        model: ObjectModel {
            QueryPage {
                width: view.cellWidth
                height: view.cellHeight

                onSearchCompleted: UI.push("search", { stage: "results", query: query })
            }

            ResultsPage {
                width: view.cellWidth
                height: view.cellHeight
            }
        }
    }

    onEnter: {
        if (parameters.stage === "results")
            view.positionViewAtEnd()
        else
            view.positionViewAtBeginning()

        ignoreStage = false
    }

    Connections {
        target: UI

        onTransition: {
            if (page === root.page)
            {
                if (parameters.stage === "results")
                    root.state = "results"
                else
                    root.state = "query"
            }
        }
    }

}
