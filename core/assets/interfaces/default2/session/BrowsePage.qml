import QtQuick 2.3
import Navigation 0.1
import Sonetta 0.1

import "../common"

FocusScope {
    id: root

    readonly property int topMargin: UI.pageTopMargin + UI.globalSpacing
    property var album
    property var artist

    signal artistBrowseRequested(var artist)
    signal albumBrowseRequested(var album)

    Loader {
        id: loader
        focus: true
        anchors {
            fill: parent
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }
    }

    Connections {
        target: loader.item
        ignoreUnknownSignals: true

        onArtistBrowseRequested: root.artistBrowseRequested(artist)
        onAlbumBrowseRequested: root.albumBrowseRequested(album)
    }

    function reset() {
        loader.source = ""
    }

    function loadArtist(artist) {
        loader.setSource("browse/artistbrowse.qml", { "artist": artist, "focus": true })
    }

    function loadAlbum(album) {
        loader.setSource("browse/albumbrowse.qml", { "album": album, "focus": true })
    }
}
