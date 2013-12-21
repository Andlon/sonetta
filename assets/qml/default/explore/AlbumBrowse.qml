import QtQuick 2.2
import Sonetta 0.1
import "../common"

FocusScope {
    focus: true
    property alias album: albumModel.album

    TrackView {
        focus: true
        model: albumModel
        anchors {
            fill: parent
        }

        onTrackPlayed: player.queue.updateContext(albumModel.album, modelIndex)
    }

    AlbumModel {
        id: albumModel
    }
}
