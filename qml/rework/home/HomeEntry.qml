import QtQuick 2.0
import sonata 0.1

Item {
    id: root

    property alias label: textLabel.text
    property alias imageWidth: image.width
    property alias imageHeight: image.height
    property alias imageSource: image.source

    signal selected

    opacity: activeFocus ? 1 : 0.35

    Behavior on opacity {
        SmoothedAnimation { duration: 150; velocity: -1 }
    }

    width: childrenRect.width
    height: childrenRect.height

    Column {
        width: childrenRect.width
        height: childrenRect.height

        Image {
            id: image
            opacity: 0.5
        }

        Text {
            id: textLabel
            color: ui.colors.headerText
            font: ui.fonts.h1

            anchors.horizontalCenter: image.horizontalCenter
        }

    }

    Navigation.onOk: {
        root.selected()
    }
}
