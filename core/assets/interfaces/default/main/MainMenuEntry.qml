import QtQuick 2.2
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

Item {
    id: root

    property string page
    property bool selected: false

    states: [
        State {
            when: selected && root.activeFocus
            PropertyChanges { target: label; color: ui.colors.highlightText }
        },
        State {
            when: !selected || !root.activeFocus
            PropertyChanges { target: label; color: ui.colors.standard }
        }
    ]

    transitions: Transition {
        ColorAnimation {
            duration: ui.misc.globalAnimationTime
            easing.type: Easing.InOutQuad
        }
    }

    anchors {
        left: parent.left
        right: parent.right
    }

    Text {
        id: label
        anchors {
            centerIn: parent
            margins: ui.misc.globalPadding
        }

        text: {
            switch (page)
            {
            case "playlists":
                return "Playlists"
            case "nowplaying":
                return "Now Playing"
            case "search":
                return "Search"
            case "session":
                return "Session"
            case "explore":
                return "Explore"
            default:
                console.error("Error: Label requested for unknown page " + page)
                return ""
            }
        }

        font: ui.fonts.h4
    }
}
