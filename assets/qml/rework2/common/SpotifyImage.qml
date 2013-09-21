import QtQuick 2.1

Image {
    property url uri
    onUriChanged: source = "image://sp/" + uri
}
