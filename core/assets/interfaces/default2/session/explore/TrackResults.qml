import QtQuick 2.3
import Sonetta 0.1

import "../../common"

FocusScope {
    id: root
    property alias model: view.model

    signal albumBrowseRequested(var album)
    signal artistBrowseRequested(var artist)

    Section {
        header: "Results - Tracks"
        anchors.fill: parent
        showFrame: false
        padding: 0

        TrackView {
            id: view
            anchors.fill: parent
            focus: true
            context: model.list

            onAlbumBrowseRequested: root.albumBrowseRequested(album)
            onArtistBrowseRequested: root.artistBrowseRequested(artist)
        }
    }
}
