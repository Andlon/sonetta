import QtQuick 2.1
import Sonetta 0.1

import "../common"

FocusScope {
    TrackView {
        id: view

        anchors.fill: parent
        focus: true

        model: search.tracks

        onTrackPlayed: player.queue.updateContext(search.tracks.list, modelIndex)

        Connections {
            target: search

            onQueryChanged: view.currentIndex = 0
        }
    }
}
