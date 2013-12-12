import QtQuick 2.2
import QtGraphicalEffects 1.0
import Sonetta 0.1

Item {
    property alias source: image.source

    signal pressed

    Image {
        id: image
        anchors.fill: parent
        visible: !parent.activeFocus
        cache: true

        sourceSize.width: parent.width
        sourceSize.height: parent.height
    }

    ColorOverlay {
        anchors.fill: parent
        source: image
        visible: parent.activeFocus
        color: ui.colors.highlight
        cached: false
    }
}
