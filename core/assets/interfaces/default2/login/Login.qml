import QtQuick 2.2
import Sonetta 0.1

import "../common"

FocusScope {
    id: root

    signal loginSuccessful

    focus: true
    state: "initial"
    states: [
        State {
            name: "initial"
            PropertyChanges { target: logo; state: "displaced" }
            PropertyChanges { target: disclaimer; opacity: 0 }
            PropertyChanges { target: credentials; opacity: 0 }
            PropertyChanges { target: header; opacity: 0 }
            PropertyChanges { target: separator; opacity: 0 }
        },
        State {
            name: "splash"
            PropertyChanges { target: logo; state: "splash" }
            PropertyChanges { target: header; opacity: 0 }
            PropertyChanges { target: credentials; opacity: 0; }
            PropertyChanges { target: separator; height: 0 }
            PropertyChanges { target: disclaimer; opacity: 1 }
        },
        State {
            name: "login"
            PropertyChanges { target: logo; state: "login"; opacity: 1 }
            PropertyChanges { target: credentials; opacity: 1 }
            PropertyChanges { target: header; opacity: 1 }
            PropertyChanges { target: separator; height: credentials.height * 1.2 }
            PropertyChanges { target: disclaimer; opacity: 1 }
            PropertyChanges { target: logo; opacity: 1 }
        },
        State {
            name: "waiting"
            PropertyChanges { target: logo; state: "hidden" }
            PropertyChanges { target: credentials; opacity: 0}
            PropertyChanges { target: header; opacity: 0 }
            PropertyChanges { target: separator; height: credentials.height * 1.2; opacity: 0 }
            PropertyChanges { target: disclaimer; opacity: 0 }
            StateChangeScript { script: m.checkLoginStatus() }
        }
    ]

    transitions: [
        Transition {
            from: "initial"
            to: "splash"
            SequentialAnimation {
                PauseAnimation { duration: 1000 }
                SmoothedAnimation { target: disclaimer; property: "opacity"; duration: 350; velocity: -1 }
            }
        },
        Transition {
            from: "splash"
            to: "login"
            SmoothedAnimation { duration: 500; target: separator; property: "height" }
            SequentialAnimation {
                PauseAnimation { duration: 200 }
                SmoothedAnimation { duration: 300; property: "opacity"; velocity: -1 }
            }
        },
        Transition {
            // Matches any transition to or from "waiting"
            SmoothedAnimation { duration: UI.timing.fade; property: "opacity"; velocity: -1 }
        }

    ]

    QtObject {
        id: m
        property bool reloginSuccessful: false
        property var loginError

        function finalizeSplash() {
            root.state = m.reloginSuccessful ? "waiting" : "login"
        }

        function login(username, password) {
            session.login(username, password)
            root.state = "waiting"
        }

        function checkLoginStatus() {
            if (root.state === "waiting") {
                if (session.connectionState === Session.LoggedIn || session.connectionState === Session.Offline) {
                    root.loginSuccessful()
                }
            }
        }
    }

    Column {
        id: disclaimer
        anchors {
            left: root.left
            right: root.right
            bottom: root.bottom
            margins: UI.globalSpacing
        }

        height: childrenRect.height

        Text {
            text: "Sonetta uses SPOTIFY CORE but is not endorsed, certified or otherwise approved in any way by Spotify."
            anchors.horizontalCenter: disclaimer.horizontalCenter
            font: UI.fonts.disclaimer
            color: UI.colors.label
        }

        Text {
            text: "Spotify is the registered trade mark of the Spotify Group."
            anchors.horizontalCenter: disclaimer.horizontalCenter
            font: UI.fonts.disclaimer
            color: UI.colors.label
        }
    }

    Rectangle {
        id: separator
        width: 2
        color: UI.box.borderColor
        anchors.centerIn: root
    }

    Image {
        id: logo
        anchors.horizontalCenter: root.horizontalCenter
        anchors.verticalCenter: root.verticalCenter
        anchors.margins: UI.globalSpacing
        source: "../images/sonetta_large.png"
        z: 1

        state: "displaced"
        states: [
            State {
                name: "displaced"
                PropertyChanges { target: displacement; y: 100 }
                PropertyChanges { target: logo; opacity: 0 }
            },
            State {
                name: "splash"
                PropertyChanges { target: displacement; y: 0 }
                PropertyChanges { target: logo; opacity: 1 }
            },
            State {
                name: "login"
                AnchorChanges {
                    target: logo;
                    anchors.horizontalCenter: undefined
                    anchors.right: root.horizontalCenter
                }
            },
            State {
                name: "hidden"
                PropertyChanges { target: logo; opacity: 0 }
                AnchorChanges {
                    target: logo;
                    anchors.horizontalCenter: undefined
                    anchors.right: root.horizontalCenter
                }
            }

        ]

        transitions: [
            Transition {
                from: "displaced"
                to: "splash"
                SequentialAnimation {
                    PauseAnimation { duration: 250 }
                    ParallelAnimation {
                        NumberAnimation {
                            properties: "y"
                            duration: 500
                            easing.type: Easing.InQuad
                        }
                        SequentialAnimation {
                            NumberAnimation {
                                properties: "opacity"
                                duration: 800
                                easing.type: Easing.InQuad
                            }

                            PauseAnimation { duration: 300 }
                            ScriptAction { script: m.finalizeSplash() }
                        }
                    }
                }
            },
            Transition {
                from: "splash"
                to: "login"
                AnchorAnimation { duration: 500; easing.type: Easing.InOutQuad }
            },
            Transition {
                to: "hidden"
                SmoothedAnimation { property: "opacity"; duration: UI.timing.fade; velocity: -1 }
            }

        ]

        transform: Translate { id: displacement }
    }


    Text {
        id: header
        font: UI.fonts.header
        color: UI.colors.dark
        text: "Spotify™ Login"

        anchors {
            bottom: credentials.top
            left: credentials.left
            leftMargin: UI.globalSpacing
            bottomMargin: UI.globalSpacing / 2
        }
    }

    Credentials {
        id: credentials
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: root.horizontalCenter
        anchors.margins: 2 * UI.globalSpacing
        focus: true

        onLoginRequested: m.login(username, password)
    }

    Connections {
        target: session
        onLoggedIn: m.checkLoginStatus();
        onLoginFailed: {
            // Implement this properly later
            m.loginError = error;
            root.state = "login"
            console.log("Login failed!")
        }
    }

    Component.onCompleted: {
        root.state = "splash";
        m.reloginSuccessful = session.relogin()
    }
}
