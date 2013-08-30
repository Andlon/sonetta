import QtQuick 2.0
import "../common"
import "../States.js" as States
import sonata 0.1

FocusScope {
    id: root
    state: "username"

    property var input

    property int animationDuration: 600

    states: [
        State {
            name: "username"
            PropertyChanges { target: username; x: 150 }
            PropertyChanges { target: password; x: root.width + 150; visible: false }
            PropertyChanges { target: root; input: username }
        },
        State {
            name: "password"
            PropertyChanges { target: username; x: - root.width + 150 }
            PropertyChanges { target: password; x: 150; visible: true }
            PropertyChanges { target: root; input: password; }
        },
        State {
            name: "pending"
            PropertyChanges { target: username; visible: false }
            PropertyChanges { target: password; x: -root.width + 150 }
            PropertyChanges { target: root; input: username; }
            PropertyChanges { target: keyboard; x: - root.width + 150 }
            PropertyChanges { target: loadingIndicator; visible: true }
            AnchorChanges { target: keyboard; anchors.left: undefined }
        }
    ]

    transitions: [
        Transition {
            from: "username"
            to: "password"
            SequentialAnimation {
                PauseAnimation { duration: animationDuration }
                PropertyAction {
                    target: username
                    property: "visible"
                    value: false
                }
            }
            SmoothedAnimation { property: "x"; duration: animationDuration; velocity: -1 }
        },
        Transition {
            from: "password"
            to: "pending"

            SequentialAnimation {
                PauseAnimation { duration: animationDuration }
                PropertyAction {
                    target: password
                    property: "visible"
                    value: false
                }
            }
            SmoothedAnimation { property: "x"; duration: animationDuration; velocity: -1 }
        }
    ]

    Image {
        // Logo
        source: "../images/sonata.png"
        anchors.bottom: loginLabel.top
        anchors.horizontalCenter: loginLabel.horizontalCenter
        anchors.bottomMargin: 20
    }

    Text {
        id: loginLabel
        text: "log in with your Spotify® Premium account"
        font.family: "Roboto"
        font.pointSize: 30
        color: "#ffffff"
        opacity: 0.4
        anchors.horizontalCenter: keyboard.horizontalCenter
        anchors.bottom: root.verticalCenter
        anchors.bottomMargin: 80
    }

    PlaceholderInputField {
        id: username
        placeholder: "username"
        width: keyboard.width
        height: 80
        font: ui.fonts.h1
        anchors.verticalCenter: root.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        color: "#ffffff"
        input.readOnly: true
        input.selectByMouse: false
        input.cursorVisible: true
    }

    PlaceholderInputField {
        id: password
        placeholder: "password"
        width: keyboard.width
        height: 80
        font: ui.fonts.h1
        anchors.verticalCenter: root.verticalCenter
        horizontalAlignment: Text.AlignHCenter
        color: "#ffffff"
        input.readOnly: true
        input.selectByMouse: false
        input.cursorVisible: true

        echoMode: TextInput.Password
    }

    VirtualKeyboard {
        id: keyboard
        focus: root.state != "pending"
        anchors.top: root.verticalCenter
        anchors.left: root.left
        anchors.leftMargin: 150
        anchors.topMargin: 100

        wrapNavigationLeft: false

        onCharacter: if (input) input.append(c)
        onBackspace: if (input) input.backspace()
        onEnter: root.nextState()

        Keys.onPressed: {
            if (input) input.append(event.text)
            event.accepted = true
        }
    }

    LoadingIndicator {
        id: loadingIndicator
        anchors.centerIn: parent
        visible: false
    }

    function nextState()
    {
        if (root.state == "username")
            root.state = "password"
        else if (root.state == "password")
            root.state = "pending"
    }

    onStateChanged: {
        if (state == "pending")
        {
            spotify.login(username.text, password.text)
        }
    }
}
