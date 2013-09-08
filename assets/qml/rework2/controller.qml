import QtQuick 2.1
import Sonetta 0.1

FocusScope {
    id: root

    Sidebar {
        id: sidebar
        width: 170

        anchors {
            left: root.left
            bottom: root.bottom
            top: root.top
        }
    }

    Loader {
        id: loader
        anchors {
            top: root.top
            left: sidebar.right
            right: root.right
            bottom: root.bottom
            margins: ui.misc.globalPadding
        }

        focus: true

        source: "playlists/playlists.qml"
    }

}
