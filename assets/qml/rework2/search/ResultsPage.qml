import QtQuick 2.1
import Sonetta 0.1

import "../common"

TrackView {
    id: view

    model: search.tracks

    onTrackPlayed: player.queue.updateContext(search.tracks.list, modelIndex)

    Connections {
        target: search

        onQueryChanged: view.currentIndex = 0
    }
}
