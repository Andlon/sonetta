import QtQuick 2.0
import QtGraphicalEffects 1.0
import sonata 0.1
import "States.js" as States

FocusScope {
    id: root

    property bool isLoggingIn: false

    Column {
        id: loginDetails
        anchors.centerIn: parent
        height: childrenRect.height
        width: 500
        spacing: 10

        visible: scale !== 0
        scale: !isLoggingIn ? 1 : 0

        Behavior on scale {
            SmoothedAnimation {
                velocity: -1
                duration: 250
            }
        }

        InputField {
            focus: true
            id: username
            label: "Username"

            width: loginDetails.width

            Keys.onTabPressed: password.forceActiveFocus()
            Keys.onUpPressed: exit.forceActiveFocus()
            Keys.onDownPressed: password.forceActiveFocus()
        }

        InputField {
            id: password
            label: "Password"
            echoMode: TextInput.Password

            width: loginDetails.width

            Keys.onTabPressed: submit.forceActiveFocus()
            Keys.onBacktabPressed: username.forceActiveFocus()
            Keys.onUpPressed: username.forceActiveFocus()
            Keys.onDownPressed: submit.forceActiveFocus()
        }

        Button {
            id: submit
            text: "Login"

            width: loginDetails.width
            Keys.onBacktabPressed: password.forceActiveFocus()
            Keys.onTabPressed: exit.forceActiveFocus()
            Keys.onUpPressed: password.forceActiveFocus()
            Keys.onDownPressed: exit.forceActiveFocus()
            onClicked: root.login()
        }

        Button {
            id: exit
            text: "Exit"

            width: loginDetails.width

            Keys.onBacktabPressed: submit.forceActiveFocus()
            Keys.onUpPressed: submit.forceActiveFocus()
            Keys.onDownPressed: username.forceActiveFocus()

            onClicked: Qt.quit()
        }

        onVisibleChanged: {
            if (visible)
                username.forceActiveFocus()
            else
            {
                username.focus = false
                password.focus = false
                submit.focus = false
            }
        }

        Navigation.onOk: root.login()
    }

    Image {
        id: loadingIndicator
        anchors.centerIn: parent
        source: "images/loading.png"
        visible: isLoggingIn

        NumberAnimation {
            target: loadingIndicator
            property: "rotation"
            from: 0
            to: 360
            duration: 1500
            running: true
            loops: Animation.Infinite
        }
    }

    Text {
        id: disclaimer
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: root.bottom
        anchors.margins: 15

        color: Qt.tint(ui.colors.text, "#33000000")
        font.family: ui.fonts.defaultFamily
        font.pointSize: ui.fonts.defaultSize * 0.7
        visible: opacity != 0

        text: "This product uses SPOTIFY CORE but is not endorsed, certified or otherwise approved\n"
              + "in any way by Spotify. Spotify is the registered trade mark of the Spotify Group."

        SequentialAnimation {
            id: disclaimerAnimation
            NumberAnimation {
                target: disclaimer
                property: "opacity"
                from: 0
                to: 1
                duration: 2000
            }
            PauseAnimation { duration: 1500 }
            NumberAnimation {
                target: disclaimer
                property: "opacity"
                from: 1
                to: 0
                duration: 7000
            }
        }

        Component.onCompleted: disclaimerAnimation.start()
    }

    function login()
    {
        if (username.text.length > 0 && password.text.length > 0)
        {
            root.isLoggingIn = true
            spotify.login(username.text, password.text)
        }
    }

    Connections {
        target: spotify

        onConnectionStateChanged: {
            if (spotify.connectionState === Spotify.LoggedInState)
            {
                // Login successful, change ui state
                // Clear states to avoid being able to go back to login
                ui.clearStates()
                ui.setState(States.createHome())
            }
        }
    }

    Component.onCompleted: username.forceActiveFocus()
}
