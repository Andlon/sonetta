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

        anchors {
            top: root.top
            left: sidebar.right
            right: root.right
            bottom: root.bottom
            margins: ui.misc.globalPadding
        }

        delegate: Loader {
            source: modelData
            focus: true
        }

        Navigation.onLeft: sidebar.focus = true

        Connections {
            target: UI
            onPageChanged: pager.updateCurrentIndex()
        }

        Component.onCompleted: { currentIndex = 1; positionViewAtIndex(1) }

        function updateCurrentIndex()
        {
            var index = 0
            switch (UI.page)
            {
            case "nowplaying":
                index = 0
                break
            case "playlists":
                index = 1
                break
            case "search":
                index = 2
                break
            case "explore":
                index = 3
                break
            case "session":
                index = 4
                break
            }

            currentIndex = index;
        }
    }

    Pattern {
        id: topSection
        visible: true //opacity != 0
        // Write states for this
        opacity: UI.page === "nowplaying" ? 0 : 1
        height: childrenRect.height + 2 * ui.misc.globalPadding
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
            OpacityAnimator {
                duration: ui.misc.globalAnimationTime;
                easing.type: Easing.InOutQuad
            }
        }
    }

    Rectangle {
        anchors {
            top: topSection.bottom
            left: sidebar.right
            right: parent.right
        }

        height: 1
        color: ui.colors.light
        visible: UI.page !== "nowplaying"
    }

}
