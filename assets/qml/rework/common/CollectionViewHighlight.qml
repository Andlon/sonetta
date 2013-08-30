import QtQuick 2.1

Image {
    property var list: parent.parent

    width: list.currentItem.width
    height: list.currentItem.height
    source: "../images/highlightTile.png"
    opacity: list.activeFocus ? 1 : 0.5
    fillMode: Image.Tile
}
