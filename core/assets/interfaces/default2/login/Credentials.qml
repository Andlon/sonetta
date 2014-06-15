import QtQuick 2.2
import QtQml.Models 2.1
import Navigation 0.1

import "../common"

Menu {
    id: root

    signal loginRequested(var username, var password)
    property alias rememberLogin: rememberMe.checked

    MenuInputItem {
        id: username
        label: "Username"
        inputHeader: "Enter your Spotify™ username"
    }

    MenuInputItem {
        id: password
        label: "Password"
        inputHeader: "Enter your Spotify™ password"
        password: true
    }

    MenuCheckableItem {
        id: rememberMe
        text: "Remember me"
    }

    MenuTextItem {
        text: "Log in"

        Navigation.onOk: root.loginRequested(username.input, password.input)
    }

    MenuTextItem {
        text: "Exit"

        Navigation.onOk: Qt.quit()
    }
}
