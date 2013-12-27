import QtQuick 2.2
import QtQml.Models 2.1
import Sonetta 0.1
import "../common"

Page {
    id: root
    page: "search"

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
                PauseAnimation { duration: 2 * ui.misc.globalAnimationTime }
                ScriptAction { script: {
                        queryPage.reset()
                        search.go(UI.parameters.query)
                    }
                }
            }
        },

        Transition {
            from: "results"
            to: "query"
            SequentialAnimation {
                PauseAnimation { duration: view.highlightMoveDuration }
                ScriptAction { script: { search.clear(); resultsPage.reset() } }
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
                id: queryPage
                width: view.cellWidth
                height: view.cellHeight

                onSearchCompleted: UI.push("search", { stage: "results", query: query })
            }

            ResultsPage {
                id: resultsPage
                width: view.cellWidth
                height: view.cellHeight
            }
        }
    }

    onEnter: {
        if (parameters.stage === "results")
            view.positionViewAtEnd()
        else
        {
            queryPage.reset()
            view.positionViewAtBeginning()
        }
    }

    Connections {
        target: UI

        onTransition: {
            if (current.page === root.page)
            {
                if (current.parameters.stage === "results")
                    root.state = "results"
                else
                    root.state = "query"
            }
        }
    }

}
