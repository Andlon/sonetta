import QtQuick 2.0

ButtonBase {
    id: root

    height: image.sourceSize.height + 50

    property alias imageSource: image.source
    property alias label: label.text

    border.width: 3

    Image {
        id: image
        anchors.left: root.left
        anchors.verticalCenter: root.verticalCenter
        anchors.margins: 30

        height: sourceSize.height
        width: height
    }

    Text {
        id: label

        color: ui.colors.text
        font.family: ui.fonts.defaultFamily
        font.pointSize: ui.fonts.menuFontSize
        font.capitalization: Font.AllUppercase
        font.bold: true
        anchors.verticalCenter: root.verticalCenter

        anchors.right: root.right
        anchors.left: image.right
        anchors.margins: 15

        elide: Text.ElideRight
    }
}
