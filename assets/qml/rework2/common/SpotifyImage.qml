import QtQuick 2.1

Image {
    asynchronous: true

    property url uri
    onUriChanged: source = "image://sp/" + uri
}
