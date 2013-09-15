import QtQuick 2.1
import Sonetta 0.1

import "main"
import "common"

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
                rightMargin: 6 * ui.misc.globalPadding
            }

            clip: true
        }

        Text {
            id: pageLabel
            color: ui.colors.standard
            font: ui.fonts.h1
            text: ui.state.pageLabel
            height: contentHeight
            anchors {
                right: parent.right
                bottom: nowplaying.bottom
            }
        }
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
        model: ["nowplaying/nowplaying.qml", "playlists/playlists.qml", "search/search.qml", "discover/discover.qml"]
        //model: ["Red", "Green", "Blue"]
        focus: true
        flow: Qt.LeftToRight
        anchors {
            top: topSection.bottom
            left: sidebar.right
            right: root.right
            bottom: root.bottom
            margins: ui.misc.globalPadding
            topMargin: ui.misc.globalPadding
        }

        cacheBuffer: 10 * pager.height

        currentIndex: sidebar.currentIndex

        delegate: Loader {
            source: modelData
            width: pager.cellWidth
            height: pager.cellHeight
            focus: true
        }

        Navigation.onLeft: sidebar.focus = true

        /* What follows is a workaround for something which looks like a bug in GridView.
          It appears binding currentIndex to something doesn't necessarily cause the highlight
          to follow the initial currentItem */
        property bool startUp: true
        onCurrentItemChanged:
        {
            if (startUp) {
                positionViewAtIndex(currentIndex, GridView.Center)
                startUp = false
            }
        }
        /* End of workaround */
    }

}
