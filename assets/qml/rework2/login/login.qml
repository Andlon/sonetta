import QtQuick 2.1
import Sonetta 0.1
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

            Navigation.onDown: password.focus = true

            onComplete: password.focus = true
        }

        LoginField {
            id: password
            label: "Password"
            Navigation.onUp: username.focus = true
            Navigation.onDown: menu.focus = true
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
                Navigation.onOk: {
                    console.log("Logging in")
                    session.login(username.text, password.text, rememberMe.toggled)
                }
            }

            MenuAction {
                text: "Exit"
                Navigation.onOk:
                {
                    console.log("Quitting")
                    Qt.quit()
                }
            }

            Navigation.onUp: password.focus = true
        }

//        ToggleView {
//            id: menu
//            width: password.width
//            model: ["Login", "Exit"]
//            alternate: false

//            Navigation.onUp: password.focus = true

//            onItemPressed: {
//                if (currentIndex == 0)
//                {
//                    session.login(username.text, password.text, true)
//                }
//                else if (currentIndex == 1)
//                {
//                    Qt.quit()
//                }
//            }

//        }
    }

}
