import QtQuick 2.2
import Sonetta.Utilities 0.1
import Sonetta 0.1

import "../common"

FocusScope {
    id: root

    property bool _reloginSuccessful: false
    focus: true

    state: "initial"
    states: [
        State {
            name: "initial"
            PropertyChanges { target: disclaimer; opacity: 0 }
            PropertyChanges { target: credentials; opacity: 0 }
            PropertyChanges { target: header; opacity: 0 }
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
            PropertyChanges { target: logo; state: "login" }
            PropertyChanges { target: credentials; opacity: 1 }
            PropertyChanges { target: header; opacity: 1 }
            PropertyChanges { target: separator; height: credentials.height * 1.2 }
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
        }
    ]

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
                PropertyChanges {
                    target: displacement;
                    y: 100
                }
                PropertyChanges {
                    target: logo;
                    opacity: 0
                }
            },
            State {
                name: "splash"
                PropertyChanges {
                    target: displacement;
                    y: 0
                }
                PropertyChanges {
                    target: logo;
                    opacity: 1
                }
            },
            State {
                name: "login"
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
                            ScriptAction { script: root.finalizeSplash() }
                        }
                    }
                }
            },
            Transition {
                from: "splash"
                to: "login"
                AnchorAnimation { duration: 500; easing.type: Easing.InOutQuad }
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
    }

    GlobalStateTransition {
        id: successfulReloginTransition

        onInitialized: {
            finalize()
        }
    }

    GlobalStateTransition {
        id: loginTransition

        onInitialized: {
            root.state = "login"
            finalize()
        }
    }

    function finalizeSplash() {
        if (_reloginSuccessful)
        {
            GlobalStateMachine.push("home");
        }
        else
        {
            GlobalStateMachine.push("login")
        }
    }

    Component.onCompleted: {
        GlobalStateMachine.registerTransition("splash", "home", successfulReloginTransition);
        GlobalStateMachine.registerTransition("splash", "login", loginTransition);

        _reloginSuccessful = session.relogin()
        root.state = "splash"
    }
}
