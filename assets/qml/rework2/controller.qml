import QtQuick 2.1
import Sonetta 0.1

FocusScope {
    id: root

    Item {
        id: topSection
        visible: ui.state.showTopSection
        height: childrenRect.height

        anchors {
            top: root.top
            right: root.right
            left: sidebar.right
            margins: ui.misc.globalPadding
        }

        NowPlaying {
            id: nowplaying
            anchors {
                left: parent.left
                right: pageLabel.left
                margins: ui.misc.globalPadding
                rightMargin: 6 * ui.misc.globalPadding
            }
        }

        Text {
            id: pageLabel
            color: ui.colors.standard
            font: ui.fonts.h1
            text: ui.state.pageLabel
            anchors {
                top: parent.top
                right: parent.right
                margins: ui.misc.globalPadding
            }
            clip: true
        }
    }

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
            top: topSection.bottom
            left: sidebar.right
            right: root.right
            bottom: root.bottom
            margins: ui.misc.globalPadding
        }

        focus: true

        source: {
            if (ui.state.page === "playlists")
                return "playlists/playlists.qml"
        }
    }

}
