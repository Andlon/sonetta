import QtQuick 2.3

Item {
    id: root
    property url uri
    property alias placeholder: placeholder.source

    property alias fillMode: image.fillMode
    property alias sourceSize: image.sourceSize
    property alias verticalAlignment: image.verticalAlignment
    property alias horizontalAlignment: image.horizontalAlignment
    property alias status: image.status

    width: image.sourceSize.width
    height: image.sourceSize.height

    Image {
        id: placeholder
        visible: source != "" && image.status != Image.Ready
        cache: true
        fillMode: image.fillMode
        asynchronous: false
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
