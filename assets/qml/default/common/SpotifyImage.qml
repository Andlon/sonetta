import QtQuick 2.2

Item {
    property url uri
    property alias placeholder: placeholder.source

    property alias fillMode: image.fillMode
    property alias sourceSize: image.sourceSize

    width: image.sourceSize.width
    height: image.sourceSize.height

    Image {
        id: placeholder
        visible: image.status != Image.Ready
        cache: true
        fillMode: image.fillMode

        anchors.fill: parent
    }

    Image {
        id: image
        asynchronous: true
        visible: image.status == Image.Ready
        source: uri != "" ? "image://sp/" + uri : ""
        anchors.fill: parent
    }
}
