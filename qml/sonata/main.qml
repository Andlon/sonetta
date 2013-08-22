import QtQuick 2.0
import sonata 0.1
import "States.js" as States

FocusScope {
    id: root

    Image {
        source: "images/background.png"
        fillMode: Image.Pad
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignTop
        anchors.fill: parent
    }

    PageView {
        id: pageView
        anchors {
            top: root.top
            left: root.left
            right: root.right
            bottom: nowPlaying.top
        }

        focus: true
    }

    NowPlayingPanel {
        id: nowPlaying
        anchors.left: root.left
        anchors.right: root.right

        y: trackIsValid ? root.height - nowPlaying.height : root.height

        Behavior on y {
            NumberAnimation { duration: 300 }
        }
    }

    Component.onCompleted:
    {
        // Set font properties
        var fonts = {};
        fonts.defaultFamily = "Roboto";
        fonts.defaultSize = 19;
        fonts.headerFamily = "Roboto";
        fonts.headerSize = 26;
        fonts.headerCapitalization = Font.AllUppercase;
        fonts.menuFontSize = 25

        ui.setFontProperties(fonts);

        // Set color properties
        var colors = {}
        colors.buttonBackground = "#7E7E7E";
        colors.background = "#2E2E2E"
        colors.inputBackground = "#eeeeee"
        colors.inputText = "#E95F14";
        colors.border = "#4b4b4b";
        colors.text = "#E9A613";
        colors.headerText = "#eeeeee"
        colors.playbackText = "#c4c4c4"

        ui.setColorProperties(colors);

        // Set initial state to login page
        ui.setState(States.createLogin());
    }

    Connections {
        target: ui

        onStateChanged: {
            var state = ui.state;

            if (state.type === "page")
            {
                if (state.page === "login")
                    pageView.load("login.qml");
                else if (state.page === "home")
                    pageView.load("home.qml");
                else if (state.page === "playlists")
                    pageView.load("playlists.qml");
                else if (state.page === "artistbrowse")
                    pageView.load("artistbrowse.qml");
                else if (state.page === "albumbrowse")
                    pageView.load("albumbrowse.qml");
                else
                    console.log("Unknown page requested.");
            }
        }
    }

    Connections {
        target: spotify

        onConnectionStateChanged: {
            if (spotify.connectionState === Spotify.LoggedOutState)
            {
                ui.clearStates()
                ui.setState(States.createLogin())
            }
        }
    }

    //Navigation.onBack: { console.log("Back!"); ui.previousState() }

    //Keys.onSpacePressed: { console.log("Back"); ui.previousState() }
    Keys.onPressed: {
        if (event.key === Qt.Key_Backspace)
            ui.previousState()
    }

    Keys.onSpacePressed: {
        if (spotify.playbackState === Spotify.PlayState)
            spotify.pause()
        else
            spotify.play()
    }
}
