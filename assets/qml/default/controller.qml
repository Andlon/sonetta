import QtQuick 2.2
import Sonetta 0.1

import "main"
import "common"

FocusScope {
    id: root

    Binding {
        target: ui.misc
        property: "topSectionHeight"
        value: topSection.height + 2 * topSection.anchors.margins
    }

    Sidebar {
        id: sidebar
        width: 220

        anchors {
            left: root.left
            bottom: root.bottom
            top: root.top
        }

        Navigation.onRight: pager.focus = true
    }

    PageView {
        id: pager
        model: ["nowplaying/nowplaying.qml", "playlists/playlists.qml",
            "search/search.qml", "explore/explore.qml", "session/session.qml"]
        focus: true
        flow: Qt.LeftToRight
        anchors {
            top: root.top
            left: sidebar.right
            right: root.right
            bottom: root.bottom
            margins: ui.misc.globalPadding
        }

        // Make sure we buffer everything (tweak this in the future?)
        cacheBuffer: 5 * pager.height
        currentIndex: sidebar.currentIndex

        delegate: Loader {
            source: modelData
            width: pager.cellWidth
            height: pager.cellHeight
            focus: true
        }

        Navigation.onLeft: sidebar.focus = true
    }

    Pattern {
        id: topSection
        visible: opacity != 0
        // Write states for this
        opacity: UI.page === "nowplaying" ? 0 : 1
        height: childrenRect.height + ui.misc.globalPadding
        pattern: "medium"

        anchors {
            top: root.top
            right: root.right
            left: sidebar.right
        }

        NowPlaying {
            id: nowplaying
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                rightMargin: 6 * ui.misc.globalPadding
                margins: ui.misc.globalPadding
            }

            clip: true
        }

        Behavior on opacity {
            SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
        }
    }

}
