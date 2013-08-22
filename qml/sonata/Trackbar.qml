import QtQuick 2.0

Item {
    id: root
    property int duration: 0
    property int position: 0

    property alias backgroundColor: background.color
    property alias border: background.border

    readonly property real progress: duration !== 0 ? position / duration : 0

    Rectangle {
        id: background
        anchors.fill: parent
        color: ui.colors.background
        border.width: 2
        border.color: ui.colors.border

        radius: background.height / 2
    }

    Rectangle {
        id: filler
        anchors.left: background.left
        anchors.top: background.top
        anchors.bottom: background.bottom
        anchors.right: tracker.right
        anchors.margins: 2

        radius: height / 2
        color: ui.colors.buttonBackground
    }

    Rectangle {
        id: tracker
        x: root.progress * (background.width - width - 2 * anchors.margins) + anchors.margins
        anchors.top: background.top
        anchors.bottom: background.bottom
        anchors.margins: 2
        width: 30
        radius: height / 2
        color: ui.colors.playbackText

        Behavior on x {
            NumberAnimation { duration: 250 }
        }
    }
}
