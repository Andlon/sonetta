import QtQuick 2.2
import Sonetta 0.1
import Sonetta.Utilities 0.1
import Navigation 0.1
import "common" 0.1

FocusScope {
    id: main
    focus: true

    states: [
        State {
            name: "login"
            PropertyChanges { target: mainInterfaceLoader; source: "login/login.qml"; }
        },
        State {
            name: "home"
            PropertyChanges { target: mainInterfaceLoader; source: "home/home.qml"; }
        }
    ]

    MainContent {
        anchors.fill: parent

        Pattern {
            // Background
            anchors.fill: parent
            pattern: UI.globalBackgroundPattern
        }

        Loader {
            id: mainInterfaceLoader
            anchors.fill: parent
            focus: true
        }
    }

    Connections {
        target: GlobalStateMachine
        onEnter: {
            switch (state)
            {
            case "splash":
                main.state = "login";
                break;
            case "home":
                main.state = "home";
                break;
            }
        }
    }

    Component.onCompleted: {
        GlobalStateMachine.initialize("splash");
    }
}
