import QtQuick 2.2
import QtQml.Models 2.1
import Sonetta 0.1

import "../common"

Page {
    id: root
    page: "playlists"

    state: "container"
    states: [
        State {
            name: "container"
            PropertyChanges { target: pager; currentIndex: 0 }
        },
        State {
            name: "playlists"
            PropertyChanges { target: pager; currentIndex: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "container"
            to: "playlists"
            SequentialAnimation {
                PauseAnimation { duration: 2 * ui.misc.globalAnimationTime }
                ScriptAction { script: playlistStage.playlist = UI.parameters.playlist }
            }
        },
        Transition {
            from: "playlists"
            to: "container"
            SequentialAnimation {
                PauseAnimation { duration: 2 * ui.misc.globalAnimationTime }
                ScriptAction { script: playlistStage.playlist = Factory.createPlaylist() }
            }
        }
    ]

    PageView {
        id: pager
        anchors {
            fill: parent
            topMargin: ui.misc.topSectionHeight
        }

        focus: true
        currentIndex: 0
        vertical: false
        clip: true

        model: ObjectModel {
            ContainerPage {
                id: view
                width: pager.cellWidth
                height: pager.cellHeight
                focus: true

                onPlaylistSelected: UI.push("playlists", { stage: "playlists", playlist: playlist })
            }

            PlaylistPage {
                id: playlistStage

                width: pager.cellWidth
                height: pager.cellHeight
            }
        }
    }

    onEnter: {
        if (parameters.stage === "playlists")
        {
            pager.positionViewAtEnd()
        }
        else
        {
            pager.positionViewAtBeginning()
        }
    }

    Connections {
        target: UI

        onTransition: {
            if (page === root.page)
            {
                if (parameters.stage === "playlists")
                    root.state = "playlists"
                else
                    root.state = "container"
            }
        }
    }
}
