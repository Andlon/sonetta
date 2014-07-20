import QtQuick 2.3
import Sonetta 0.1

import "../."
import "../Time.js" as Time

CollectionView {
    id: root
    delegate: SingleRowTrackDelegate {
        onHeightChanged: root.delegateHeight = height
    }

    onItemPressed: {
        player.play(model.track)
        if (root.model.playlist)
            player.queue.updateContext(root.model.playlist, index)
    }
}
