import QtQuick 2.2
import Sonetta 0.1

import "../common"
import "../common/States.js" as States

Item {
    id: root

    property string page
    property bool selected: ui.state.page === page

    height: 150
    anchors {
        left: parent.left
        right: parent.right
    }

    Text {
        anchors {
            centerIn: parent
            margins: ui.misc.globalPadding
        }

        text: {
            switch (page)
            {
            case "playlists":
                return "playlists"
            case "nowplaying":
                return "now playing"
            case "search":
                return "search"
            case "session":
                return "session"
            case "explore":
                return "explore"
            default:
                console.error("Error: Label requested for unknown page " + page)
                return ""
            }
        }

        color: ui.colors.standard
        font: ui.fonts.h4
    }
}
