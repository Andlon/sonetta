import QtQuick 2.2

import "../common"

Column {
    height: childrenRect.height
    width: childrenRect.width

    MenuItem {
        text: "Username"
    }

    MenuItem {
        text: "Password"
    }

    MenuItem {
        text: "Remember me"
    }

    MenuItem {
        text: "Log in"
    }

    MenuItem {
        text: "Exit"
    }
}
