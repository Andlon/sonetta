import QtQuick 2.0
import sonata 0.1

import "../States.js" as States

Item {
    id: root

    property size menuImageSize: Qt.size(400, 400)

    Item {
        id: leftBackground

        anchors {
            left: root.left
            top: root.top
            bottom: root.bottom
        }

        width: root.width / 3
        HomeEntry {
            id: searchEntry
            anchors.centerIn: parent
            label: "search"
            imageWidth: menuImageSize.width
            imageHeight: menuImageSize.height
            imageSource: "../images/search.png"

            focus: true

            Navigation.onRight: playlistsEntry.forceActiveFocus()
            onSelected: ui.pushState(States.createSearch())
        }
    }

    Image {
        id: middleBackground
        source: "../images/lightDarkTile.png"
        fillMode: Image.Tile

        anchors {
            left: leftBackground.right
            right: rightBackground.left
            top: root.top
            bottom: root.bottom
        }

        HomeEntry {
            id: playlistsEntry
            anchors.centerIn: parent
            label: "playlists"
            imageWidth: menuImageSize.width
            imageHeight: menuImageSize.height
            imageSource: "../images/playlist.png"

            Navigation.onLeft: searchEntry.forceActiveFocus()
            Navigation.onRight: discoverEntry.forceActiveFocus()
        }
    }

    Item {
        id: rightBackground

        anchors {
            right: root.right
            top: root.top
            bottom: root.bottom
        }

        width: root.width / 3
        HomeEntry {
            id: discoverEntry
            anchors.centerIn: parent
            label: "discover"
            imageWidth: menuImageSize.width
            imageHeight: menuImageSize.height
            imageSource: "../images/discover.png"

            Navigation.onLeft: playlistsEntry.forceActiveFocus()
        }
    }
}
