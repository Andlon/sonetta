import QtQuick 2.2
import QtQuick.Layouts 1.1
import Sonetta 0.1

import "../common"

FocusScope {
    id: root
    focus: true

    property alias artist: synopsis.artist

    ArtistSynopsis {
        id: synopsis

    }

    SpotifyImage {
        id: portrait

        anchors {
            left: menu.left
            top: root.top
            right: menu.right
        }

        height: {
            var maxHeight = root.height - menu.height - ui.misc.globalPadding
            var preferredHeight = (width / sourceSize.width) * sourceSize.height
            return Math.min(maxHeight, preferredHeight)
        }

        uri: synopsis.largePortraitUri
        fillMode: Image.PreserveAspectFit
    }

    Menu {
        id: menu
        focus: true
        width: root.width / 3

        anchors {
            left: root.left
            top: portrait.bottom
            topMargin: ui.misc.globalPadding
        }

        MenuAction { text: "Top hits" }
        MenuAction { text: "Albums" }
        MenuAction { text: "Similar artists" }
        MenuAction { text: "Biography" }

        KeyNavigation.right: topHits
    }

    Text {
        id: artistName
        anchors {
            top: parent.top
            left: topHits.left
            right: topHits.right

            leftMargin: ui.misc.globalPadding
            rightMargin: ui.misc.globalPadding
        }

        text: synopsis.name
        font: ui.fonts.h2
        color: ui.colors.standard
    }

    Text {
        id: label
        anchors {
            top: artistName.bottom
            left: artistName.left
            right: artistName.right
        }

        text: "Top hits"
        font: ui.fonts.h2
        color: ui.colors.label
    }

    TrackView {
        id: topHits
        anchors {
            top: label.bottom
            right: parent.right
            left: menu.right
            bottom: parent.bottom

            leftMargin: ui.misc.globalPadding
            topMargin: 2 * ui.misc.globalPadding
        }

        model: synopsis.hits

        KeyNavigation.left: menu
    }
}
