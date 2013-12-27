import QtQuick 2.2
import Sonetta 0.1
import Navigation 0.1

import "common"
import "common/States.js" as States

Item {
    id: root

    states: [
        State {
            name: "login"
            PropertyChanges { target: loader; source: "login/login.qml" }
        },
        State {
            name: "main"
            PropertyChanges { target: loader; source: "controller.qml" }
        }
    ]

    Pattern {
        // background
        anchors.fill: parent
        pattern: "medium"
        fillMode: Image.Tile
    }

    Loader {
        id: loader
        anchors.fill: parent
        focus: true

        onSourceChanged: item.focus = true
        onStatusChanged: ui.misc.ready = status == Loader.Ready
    }

    Connections {
        target: session
        onLoggedIn: root.state = "main"
        onLoggedOut: root.state = "login"
    }

    Component.onCompleted: {
        UI.reset("playlists", {})

        // Set up UI visual configuration
        var misc = {}
        misc.globalBottomMargin = 100
        misc.globalTopMargin = 100
        misc.globalAnimationTime = 200
        misc.ready = false
        misc.topSectionHeight = 0

        // The amount of space between any two generic items
        misc.globalPadding = 30

        var colors = {}
        colors.highlight = "#84bd00"
        //colors.selected = "#424242"
        colors.selected = "#4F7000"
        colors.standard = "#ffffff"
        colors.highlightText = "#292929"
        colors.label = "#999999"

        colors.dark = "#151515"
        colors.medium = "#202020"
        colors.light = "#292929"

        var fonts = {}
        // Create default font
        var standard = {}
        standard.family = "Roboto"
        standard.pointSize = 19
        standard.weight = 40
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
        h2.weight = Font.Light
        fonts.h2 = Qt.font(h2)

        var h3 = {}
        h3.family = "Roboto"
        h3.pointSize = 25
        h3.weight = 40
        fonts.h3 = Qt.font(h3)

        var h4 = {}
        h4.family = "Roboto"
        h4.pointSize = 22
        h4.weight = 40
        fonts.h4 = Qt.font(h4)

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

    Keys.forwardTo: Nav {
        onBack: UI.pop()
        onPlay: player.play()
        onPlayPause: player.playPause()
        onPause: player.pause()
        onStop: player.pause()
        onNext: player.next()
    }

    Keys.onSpacePressed: player.playPause()
}
