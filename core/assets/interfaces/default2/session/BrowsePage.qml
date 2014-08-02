import QtQuick 2.3
import Navigation 0.1
import Sonetta 0.1

import "../common"

FocusScope {
    id: root

    readonly property int topMargin: UI.pageTopMargin + UI.globalSpacing
    property var album
    property var artist

    Loader {
        id: loader
        focus: true
        anchors {
            fill: parent
            margins: UI.globalSpacing
            topMargin: root.topMargin
        }
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
