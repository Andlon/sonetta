import QtQuick 2.0
import com.bitfraction.sonata 0.1

FocusScope {
    id: root
    width: 300
    height: childrenRect.height

    signal loginSubmitted(var username, var password)

    Column {
        height: childrenRect.height
        width: root.width
        spacing: 5

        InputField {
            id: username
            width: root.width
            label: "Username:"

            focus: true

            Keys.onTabPressed: password.forceActiveFocus()
            Navigation.onDown: password.forceActiveFocus()

            onKeyboardClosed: password.forceActiveFocus()
        }

        InputField {
            id: password
            width: root.width
            label: "Password:"

            echoMode: TextInput.Password

            Keys.onTabPressed: submit.forceActiveFocus()
            Keys.onBacktabPressed: username.forceActiveFocus()
            Navigation.onDown: submit.forceActiveFocus()
            Navigation.onUp: username.forceActiveFocus()

            onKeyboardClosed: submit.forceActiveFocus()
        }

        Button {
            id: submit
            text: "Login"
            width: root.width
            height: 45

            onClicked: root.loginSubmitted(username.text, password.text)

            Keys.onBacktabPressed: password.forceActiveFocus()
            Navigation.onUp: password.forceActiveFocus()
        }

    }

    Navigation.onOk: loginSubmitted(username.text, password.text)
}
