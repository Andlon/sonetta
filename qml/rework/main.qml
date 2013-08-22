import QtQuick 2.0
import QtQuick.Layouts 1.0
import sonata 0.1
import "States.js" as States

Item {
    id: root
    width: 800
    height: 600

    property string page

    Image {
        source: "images/background.png"
        anchors.fill: parent
    }

    PageLoader {
        id: pageLoader
        anchors {
            top: root.top
            bottom: root.bottom
            left: leftBar.right
            right: root.right
        }
    }

    NowPlaying {
        anchors.left: leftBar.right
        anchors.top: parent.top
        anchors.margins: 20
    }

    Image {
        id: leftBar

        source: "images/leftBarTile.png"
        width: 150
        fillMode: Image.Tile

        anchors {
            left: root.left
            bottom: root.bottom
            top: root.top
        }

        QuickMenu {
            id: quickMenu
            anchors.horizontalCenter: leftBar.horizontalCenter
            anchors.top: leftBar.top
            anchors.bottom: leftBar.bottom
            anchors.margins: 30

            Navigation.onRight: pageLoader.forceActiveFocus()
        }
    }

    Image {
        // Powered by spotify
        anchors.bottom: root.bottom
        anchors.right: root.right
        anchors.margins: 20
        source: "images/spotify.png"
    }

    Component.onCompleted:
    {
        // Set font properties
        var fonts = {};

        // Create default font
        var standard = {}
        standard.family = "Roboto"
        standard.pointSize = 19
        fonts.standard = Qt.font(standard)

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

        ui.setFontProperties(fonts);

        // Set color properties
        var colors = {}
        colors.defaultText = "#ffffff"
        colors.headerText = "#ffffff"
        colors.highlight = "#84bd00"

        ui.setColorProperties(colors);

        // Set miscellaneous properties
        var misc = {}
        misc.verticalContentMargin = 200
        ui.setMiscProperties(misc)

        // Set initial state to login page
        ui.initState(States.createLogin(), UiStateManagement.None);
    }

    Connections {
        target: ui

        onStateChanged: {
            var state = ui.state;

            if (direction === UiStateManagement.Forward)
                pageLoader.direction = "right"
            else if (direction === UiStateManagement.Backward)
                pageLoader.direction = "left"
            else
                pageLoader.direction = ""

            if (state.type === "page" && state.page !== root.page)
            {
                if (state.page === "login")
                    pageLoader.load("login/login.qml");
                else if (state.page === "home")
                    pageLoader.load("home/home.qml");
                else if (state.page === "playlists")
                    pageLoader.load("playlists/playlists.qml");
                else if (state.page === "search")
                    pageLoader.load("search/search.qml")
                else if (state.page === "artistbrowse")
                    pageLoader.load("artistbrowse.qml");
                else if (state.page === "albumbrowse")
                    pageLoader.load("albumbrowse.qml");
                else
                    console.log("Unknown page requested.");

                root.page = state.page
            }
        }
    }

    Navigation.onLeft: quickMenu.forceActiveFocus()
    Navigation.onBack: ui.previousState()

    Keys.onEscapePressed: {
        var page = ui.state.page

        if (page !== "login")
            ui.resetState(States.createHome())
    }

    Connections {
        target: spotify

        onConnectionStateChanged: {
            if (spotify.connectionState === Spotify.LoggedOutState)
            {
                ui.resetState(States.createLogin())
            }
            else if (spotify.connectionState === Spotify.LoggedInState) // || spotify.connectionState === Spotify.OfflineState)
            {
                ui.resetState(States.createHome(), UiStateManagement.Forward);
            }
        }
    }
}
