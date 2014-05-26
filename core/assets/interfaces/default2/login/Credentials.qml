import QtQuick 2.2
import QtQml.Models 2.1
import Navigation 0.1

import "../common"

Menu {
    id: root

    MenuInputItem {
        label: "Username"
        inputHeader: "Enter your Spotify™ username"
    }

    MenuInputItem {
        label: "Password"
        inputHeader: "Enter your Spotify™ password"
        password: true
    }

    MenuTextItem {
        text: "Remember me"
    }

    MenuTextItem {
        text: "Log in"
    }

    MenuTextItem {
        text: "Exit"

        Navigation.onOk: Qt.quit()
    }
}
