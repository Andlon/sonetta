import QtQuick 2.2

QtObject {
    property var input
    property var output

    property alias delay: timer.interval

    onInputChanged: timer.restart()

    Timer {
        id: timer
        interval: delay

        onTriggered: output = input
    }
}
