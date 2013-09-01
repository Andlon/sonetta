import QtQuick 2.1
import Sonetta 0.1
import "../common"

FocusScope {
    id: root

    state: "username"

    property var currentInput: username

    states: [
        State {
            name: "username"
            PropertyChanges { target: selectionHighlight; item: username }
            PropertyChanges { target: root; currentInput: username }
            AnchorChanges { target: keyboard; anchors.top: username.bottom }
            AnchorChanges { target: username; anchors.left: root.horizontalCenter;
                anchors.right: root.right }
            AnchorChanges { target: password; anchors.left: root.left;
                anchors.right: root.horizontalCenter }
            AnchorChanges { target: selectionHighlight;
                anchors.top: username.top; anchors.bottom: username.bottom; anchors.right: username.right }
        },
        State {
            name: "password"
            PropertyChanges { target: selectionHighlight; item: password }
            PropertyChanges { target: root; currentInput: password }
            AnchorChanges { target: keyboard; anchors.top: password.bottom }
            AnchorChanges { target: username; anchors.left: root.left;
                anchors.right: root.horizontalCenter }
            AnchorChanges { target: password; anchors.left: root.horizontalCenter;
                anchors.right: root.right }
            AnchorChanges { target: selectionHighlight;
                anchors.top: password.top; anchors.bottom: password.bottom; anchors.right: password.right }
        },
        State {
            name: "login"
            PropertyChanges { target: selectionHighlight; item: login }
            PropertyChanges { target: root; currentInput: password }
            PropertyChanges { target: selectionHighlight;
                restoreEntryValues: true; visible: false }
            PropertyChanges { target: login; focus: true }
            PropertyChanges { target: keyboard; restoreEntryValues: true; opacity: 0 }
            AnchorChanges { target: keyboard; anchors.top: password.bottom }
            AnchorChanges { target: username; anchors.left: root.left;
                anchors.right: root.horizontalCenter }
            AnchorChanges { target: password; anchors.left: root.left;
                anchors.right: root.horizontalCenter }

        }

    ]

    transitions: Transition {
        enabled: ui.misc.ready
        AnchorAnimation {
            duration: ui.misc.globalAnimationTime
        }
    }

    Item {
        id: header
        anchors {
            top: root.top
            left: root.left
            right: root.right
            margins: ui.misc.globalPadding
        }

        height: childrenRect.height

        Image {
            // logo
            source: "../images/sonetta.png"
            anchors.horizontalCenter: parent.horizontalCenter
            sourceSize.height: 150
        }
    }


    Rectangle {
        id: selectionHighlight
        property var item: username

        color: item.activeFocus ? ui.colors.highlight : ui.colors.selected
        anchors {
            left: root.left
            leftMargin: ui.misc.globalPadding
        }
    }

    LoginField {
        id: username
        label: "Username"
        //text: root.username

        anchors {
            top: header.bottom
            left: root.left
            right: root.right
            margins: ui.misc.globalPadding
        }
    }

    LoginField {
        id: password
        label: "Password"
        //text: root.password
        anchors {
            top: username.bottom
            left: root.left
            right: root.horizontalCenter
            margins: ui.misc.globalPadding
        }
    }

    Text {
        id: login
        text: "LOG IN"
        font: ui.fonts.h1
        color: activeFocus ? ui.colors.highlight : ui.colors.standard
        anchors {
            top: password.bottom
            left: root.left
            right: root.horizontalCenter
            margins: 2 * ui.misc.globalPadding
        }

        Navigation.onOk: {
            session.login(username.text, password.text, true)
        }
    }

    Text {
        text: "Requires a subscription to Spotify© Premium"
        font: ui.fonts.standard
        color: ui.colors.standard
        anchors {
            bottom: disclaimer.top
            horizontalCenter: root.horizontalCenter
            margins: 2 * ui.misc.globalPadding
        }
    }

    Text {
        id: disclaimer
        text: "This product uses Music by Spotify but is not endorsed, certified or otherwise approved in any way by Spotify. Spotify is the registered trade mark of the Spotify Group."
        font: ui.fonts.disclaimer
        color: ui.colors.label
        wrapMode: Text.Wrap
        anchors {
            bottom: root.bottom
            horizontalCenter: root.horizontalCenter
            margins: 2 * ui.misc.globalPadding
        }
    }

    VirtualKeyboard {
        id: keyboard
        anchors {
            left: root.horizontalCenter
            margins: ui.misc.globalPadding
        }

        Behavior on opacity {
            SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
        }

        focus: true

        highlightColor: ui.colors.highlight
        color: ui.colors.standard

        onCharacter: currentInput.text += c
        onEnter: {
            if (root.state == "username")
                root.state = "password"
            else if (root.state == "password")
                root.state = "login"
            else
                root.state = "username"
        }
    }


}
