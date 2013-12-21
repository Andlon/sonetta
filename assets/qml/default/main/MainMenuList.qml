import QtQuick 2.2
import QtQml.Models 2.1
import Sonetta 0.1
import "../common"
import "../common/States.js" as States

ListView {
    id: root
    model: [ "nowplaying", "playlists", "search", "explore", "session" ]
    currentIndex: {
        switch (UI.page)
        {
        case "nowplaying":
            return 0
        case "playlists":
            return 1
        case "search":
            return 2
        case "explore":
            return 3
        case "session":
            return 4
        }
    }

    delegate: MainMenuEntry {
        id: entry
        page: modelData
        height: root.height / root.count

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

    Navigation.onDown: updateIndex(currentIndex + 1)
    Navigation.onUp: updateIndex(currentIndex - 1)

    function updateIndex(index)
    {
        if (index >= 0 && index < count)
        {
            UI.reset(root.model[index], {})
        }
    }
}
