import QtQuick 2.1
import Sonetta 0.1

import "common"

Item {
    id: root

    states: [
        State {
            name: "login"
        },
        State {
            name: "main"
        }

    ]

    Image {
        // background
        anchors.fill: parent
        source: "images/medium.png"
        fillMode: Image.Tile
    }

    Image {
        // Powered by Spotify
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: ui.misc.globalPadding
        source: "images/spotify.png"
    }

    Loader {
        anchors.fill: parent
        source: root.state == "login" ? "login/login.qml" : "controller.qml"
    }

    Connections {
        target: session
        onLoggedIn: root.state = "main"
        onLoggedOut: root.state = "login"
    }

    Component.onCompleted: {
        // Set up UI visual configuration
        var misc = {}
        misc.globalBottomMargin = 100
        misc.globalTopMargin = 100

        // The amount of space between any two generic items
        misc.globalPadding = 20

        var colors = {}
        colors.highlight = "#84bd00"
        colors.standard = "#ffffff"

        ui.setMiscProperties(misc)
        ui.setColorProperties(colors)

        if (session.relogin())
        {
            state = "main"
        }
        else
        {
            state = "login"
        }
    }
}
