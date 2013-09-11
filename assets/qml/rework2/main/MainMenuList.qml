import QtQuick 2.1
import QtQml.Models 2.1
import Sonetta 0.1
import "../common"
import "../common/States.js" as States

ListView {
    id: root
    model: [ "nowplaying", "playlists", "search", "discover" ]
    currentIndex: 1

    delegate: MenuEntry {
        id: entry
        page: modelData

        Rectangle {
            anchors {
                top: entry.top
                left: entry.left
                right: entry.right
            }
            height: 1
            z: -1
            parent: root.contentItem
            color: ui.colors.light
        }

        Rectangle {
            visible: index == root.count - 1
            anchors {
                bottom: entry.bottom
                left: entry.left
                right: entry.right
            }
            height: 1
            z: -1
            parent: root.contentItem
            color: ui.colors.light
        }
    }

    highlightMoveDuration: ui.misc.globalAnimationTime
    highlight: Rectangle {
        width: root.activeFocus ? root.width : ui.misc.globalPadding / 2
        color: ui.colors.highlight

        Behavior on width {
            SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
        }
    }

    Navigation.onDown: incrementCurrentIndex()
    Navigation.onUp: decrementCurrentIndex()

    onCurrentIndexChanged: {
        if (currentItem)
        {
            ui.resetState(States.createPage(ui.state, currentItem.page))
        }
    }
}
