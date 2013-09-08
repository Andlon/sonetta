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

    //    Image {
    //        // Powered by Spotify
    //        anchors.bottom: parent.bottom
    //        anchors.right: parent.right
    //        anchors.margins: ui.misc.globalPadding
    //        source: "images/spotify.png"
    //    }

    Loader {
        anchors.fill: parent
        focus: true

        source: {
            if (root.state == "login")
                return "login/login.qml"
            else if (root.state == "main")
                return "controller.qml"
            else
                return ""
        }

        onSourceChanged: item.focus = true

        onStatusChanged: ui.misc.ready = status == Loader.Ready
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
        misc.globalAnimationTime = 150
        misc.ready = false

        // The amount of space between any two generic items
        misc.globalPadding = 20

        var colors = {}
        colors.highlight = "#84bd00"
        //colors.selected = "#424242"
        colors.selected = "#4F7000"
        colors.standard = "#ffffff"
        colors.label = "#cccccc"

        var fonts = {}
        // Create default font
        var standard = {}
        standard.family = "Roboto"
        standard.pointSize = 19
        fonts.standard = Qt.font(standard)

        // Create disclaimer font
        var disclaimer = {}
        disclaimer.family = "Roboto"
        disclaimer.pointSize = 16
        fonts.disclaimer = Qt.font(disclaimer)

        // Create header fonts
        var h1 = {}
        h1.family = "Roboto"
        h1.pointSize = 50
        fonts.h1 = Qt.font(h1)

        var h2 = {}
        h2.family = "Roboto"
        h2.pointSize = 35
        fonts.h2 = Qt.font(h2)

        var h3 = {}
        h3.family = "Roboto"
        h3.pointSize = 25
        fonts.h3 = Qt.font(h3)

        ui.setMiscProperties(misc)
        ui.setColorProperties(colors)
        ui.setFontProperties(fonts);

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
