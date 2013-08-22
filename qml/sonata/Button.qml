import QtQuick 2.0

ButtonBase {
    id: root
    property alias text: label.text

    Text {
        id: label
        anchors.centerIn: parent
        color: ui.colors.text
        font.family: ui.fonts.defaultFamily
        font.pointSize: ui.fonts.defaultSize
    }
}
