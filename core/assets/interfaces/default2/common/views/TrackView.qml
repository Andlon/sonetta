import QtQuick 2.3
import Sonetta 0.1

import ".."
import "../Time.js" as Time

CollectionView {
    id: root

    property var context
    property alias actions: contextMenu.actions

    signal unhandledAction(var id)
    signal artistBrowseRequested(var artist)
    signal albumBrowseRequested(var album)

    delegate: SingleRowTrackDelegate {
        onHeightChanged: root.delegateHeight = height
    }

    onItemPressed: {
        contextMenu.track = model.track
        contextMenu.artist = model.artist
        contextMenu.album = model.album
        contextMenu.index = index
        contextMenu.activate()
    }

    ContextMenuDialog {
        id: contextMenu
        menuHeader: "Track Actions"

        property var track
        property var album
        property var artist
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
            case "browsealbum":
                root.albumBrowseRequested(album)
                break
            case "browseartist":
                root.artistBrowseRequested(artist)
                break
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
    }
}
