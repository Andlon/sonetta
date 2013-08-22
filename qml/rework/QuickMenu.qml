import QtQuick 2.0
import sonata 0.1

import "common/States.js" as States

FocusScope {
    id: root

    height: 500
    width: childrenRect.width

    property int actionWidth: 80
    property int actionHeight: 80


    Column {
        anchors.bottom: root.bottom
        width: childrenRect.width

        spacing: 20

        Image {
            id: homeAction

            width: root.actionWidth
            height: root.actionHeight
            source: "images/home.png"
            opacity: activeFocus ? 1 : 0.35
            focus: true

            Navigation.onDown: settingsAction.forceActiveFocus()
            Navigation.onOk: if (ui.state.page && ui.state.page !== "login") ui.resetState(States.createHome())
        }

        Image {
            id: settingsAction

            width: root.actionWidth
            height: root.actionHeight
            source: "images/settings.png"
            opacity: activeFocus ? 1 : 0.35

            Navigation.onDown: logoutAction.forceActiveFocus()
            Navigation.onUp: homeAction.forceActiveFocus()
        }

        Image {
            id: logoutAction

            width: root.actionWidth
            height: root.actionHeight
            source: "images/logout.png"
            opacity: activeFocus ? 1 : 0.35

            Navigation.onUp: settingsAction.forceActiveFocus()
            Navigation.onOk: spotify.logout()
        }

    }

}
