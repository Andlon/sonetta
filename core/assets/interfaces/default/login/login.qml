import QtQuick 2.2
import Sonetta 0.1
import Navigation 0.1
import "../common"

FocusScope {
    id: root

    Image {
        id: logo
        anchors {
            top: root.top
            horizontalCenter: root.horizontalCenter
        }

        source: "../images/sonetta_large.png"
        height: 350
        width: height
    }

    Column {
        anchors {
            top: logo.bottom
            horizontalCenter: root.horizontalCenter
        }

        width: childrenRect.width

        spacing: ui.misc.globalPadding
        LoginField {
            id: username
            label: "Username"
            focus: true

            Keys.forwardTo: Nav { onDown: password.focus = true }

            onComplete: password.focus = true
        }

        LoginField {
            id: password
            label: "Password"
            Keys.forwardTo: Nav {
                onUp: username.focus = true
                onDown: menu.focus = true
            }
            onComplete: menu.focus = true
        }

        Menu {
            id: menu
            width: password.width

            MenuAction {
                id: rememberMe
                text: "Remember me"
                toggleable: true
                toggled: true
            }

            MenuAction {
                text: "Login"
                onOk: session.login(username.text, password.text, rememberMe.toggled)
            }

            MenuAction {
                text: "Exit"
                onOk: Qt.quit()
            }

            Keys.forwardTo: Nav {
                onUp: password.focus = true
            }
        }
    }

}
