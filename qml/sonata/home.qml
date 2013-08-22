import QtQuick 2.0
import "States.js" as States

FocusScope {
    id: root

    Grid {
        anchors.centerIn: parent

        columns: 2
        spacing: 10

        MainMenuEntry {
            id: playlists
            imageSource: "images/playlists.png"
            label: "Playlists"
            width: 440
            focus: true

            Keys.onTabPressed: search.forceActiveFocus()
            Keys.onBacktabPressed: logout.forceActiveFocus()
            Keys.onRightPressed: search.forceActiveFocus()
            Keys.onDownPressed: settings.forceActiveFocus()

            onClicked: root.goToPlaylists()
        }

        MainMenuEntry {
            id: search
            imageSource: "images/search.png"
            label: "Search"
            width: 440

            Keys.onTabPressed: settings.forceActiveFocus()
            Keys.onBacktabPressed: playlists.forceActiveFocus()
            Keys.onLeftPressed: playlists.forceActiveFocus()
            Keys.onDownPressed: logout.forceActiveFocus()
        }

        MainMenuEntry {
            id: settings
            imageSource: "images/settings.png"
            label: "Settings"
            width: 440

            Keys.onTabPressed: logout.forceActiveFocus()
            Keys.onBacktabPressed: search.forceActiveFocus()
            Keys.onRightPressed: logout.forceActiveFocus()
            Keys.onUpPressed: playlists.forceActiveFocus()
        }

        MainMenuEntry {
            id: logout
            imageSource: "images/playlists.png"
            label: "Logout"
            width: 440
            onClicked: spotify.logout()

            Keys.onTabPressed: playlists.forceActiveFocus()
            Keys.onBacktabPressed: settings.forceActiveFocus()
            Keys.onLeftPressed: settings.forceActiveFocus()
            Keys.onUpPressed: search.forceActiveFocus()
        }
    }

    function goToPlaylists()
    {
        ui.setState(States.createPlaylists());
    }


}
