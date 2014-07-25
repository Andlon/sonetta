import QtQuick 2.3
import Sonetta 0.1

import ".."
import "../Time.js" as Time

CollectionView {
    id: root

    property var context
    property alias actions: contextMenu.actions

    signal unhandledAction(var id)

    delegate: SingleRowTrackDelegate {
        onHeightChanged: root.delegateHeight = height
    }

    onItemPressed: {
        contextMenu.track = model.track
        contextMenu.index = index
        contextMenu.activate()
    }

    ContextMenuDialog {
        id: contextMenu
        menuHeader: "Track Actions"

        property var track
        property int index

        actions: [ [ "play", "Play now" ],
            [ "playpreserve", "Play now, keep queue" ],
            [ "enqueue", "Add to queue" ],
            [ "browsealbum", "Browse album" ],
            [ "browseartist", "Browse artist" ] ]

        onActionSelected: {
            switch (id) {
            case "play":
                player.play(track)
                updateContext()
                break;
            case "playpreserve":
                player.play(track)
                break;
            case "enqueue":
                player.enqueue(track)
                break;
            default:
                root.unhandledAction(id)
                break;
            }

            contextMenu.deactivate()
        }

        function updateContext() {
            if (root.context) {
                player.queue.updateContext(root.context, index)
            }
        }

        onActivating: contextMenu.focus = true
        onDeactivating: root.viewItem.forceActiveFocus()
    }
}
