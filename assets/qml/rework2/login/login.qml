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

        MenuView {
            id: menu
            width: password.width
            height: 200
            model: ["Login", "Exit"]

            Navigation.onUp: password.focus = true

            onItemPressed: {
                if (currentIndex == 0)
                {
                    session.login(username.text, password.text, true)
                }
                else if (currentIndex == 1)
                {
                    Qt.quit()
                }
            }

        }
    }

}
