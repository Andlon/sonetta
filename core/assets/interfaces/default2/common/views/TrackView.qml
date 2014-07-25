import QtQuick 2.3
import Sonetta 0.1

import "../."
import "../Time.js" as Time

CollectionView {
    id: root

    property var context

    delegate: SingleRowTrackDelegate {
        onHeightChanged: root.delegateHeight = height
    }

    onItemPressed: {
        player.play(model.track)
        if (context)
            player.queue.updateContext(context, index)
    }
}
