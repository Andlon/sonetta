import QtQuick 2.2
import Sonetta 0.1
import Sonetta.Utilities 0.1
import Navigation 0.1
import "common" 0.1

import "common/Dialog.js" as Dialog

import "login"
import "session"

FocusScope {
    id: root
    focus: true

    states: [
        State {
            name: "login"
            PropertyChanges { target: sessionInterface; opacity: 0 }
            PropertyChanges { target: loginInterface; opacity: 1; focus: true }
        },
        State {
            name: "session"
            PropertyChanges { target: sessionInterface; opacity: 1; focus: true }
            PropertyChanges { target: loginInterface; opacity: 0 }
        }
    ]
    state: "login"

    transitions: Transition {
        SmoothedAnimation { property: "opacity"; duration: UI.timing.fade; velocity: -1 }
    }

    FocusScope {
        id: mainContent
        anchors.fill: parent
        focus: true

        Pattern {
            // Background
            anchors.fill: parent
            pattern: UI.globalBackgroundPattern
        }

        // TODO: Move Login and Session into Loaders, such that only one is loaded at a time
        Login {
            id: loginInterface
            anchors.fill: parent
            onLoginSuccessful: root.state = "session"
        }

        Session {
            id: sessionInterface
            anchors.fill: parent
        }
    }

    Connections {
        target: session
        onLoggedOut: root.state = "login"
        onConnectionStateChanged: {
            switch (session.connectionState) {
            case session.LoggedOut:
            case session.Undefined:
                root.state = "login"
                break
            }
        }
    }

    Component.onCompleted: {
        Dialog.setRootItem(root)
        Dialog.setContentItem(mainContent)
    }
}
