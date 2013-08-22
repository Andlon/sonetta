import QtQuick 2.0
import sonata 0.1

FocusScope {
    id: root

    property alias menuWidth: leftContainer.width
    property alias menuHeader: menuHeader.text
    property alias menuBackgroundColor: leftBackground.color
    property bool showMenu: true
    property alias listHeader: listHeader.text
    property alias listBackgroundColor: rightBackground.color
    property alias model: list.model
    property alias listDelegate: list.delegate
    property alias menuDelegate: menu.sourceComponent
    property alias listHighlight: list.highlight
    property alias listCurrentIndex: list.currentIndex
    property alias listCurrentItem: list.currentItem
    property alias listCount: list.count
    property int spacing: 20
    readonly property real listWidth: list.width
    readonly property real listHeight: list.height

    Item {
        id: leftContainer

        visible: root.showMenu

        anchors.left: root.left
        anchors.top: root.top
        anchors.bottom: root.bottom

        width: 400

        Rectangle {
            id: leftBackground
            color: ui.colors.background
            border.width: 2
            border.color: ui.colors.border
            radius: 25

            anchors.fill: parent
        }

        Text {
            id: menuHeader

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 40

            color: ui.colors.headerText
            font.family: ui.fonts.headerFamily
            font.pointSize: ui.fonts.headerSize
            font.capitalization: ui.fonts.headerCapitalization
        }

        Loader {
            id: menu

            anchors.top: menuHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 25
        }
    }

    Item {
        id: rightContainer

        anchors.left: root.showMenu ? leftContainer.right : root.left
        anchors.top: root.top
        anchors.bottom: root.bottom
        anchors.right: root.right
        anchors.leftMargin: root.spacing

        Rectangle {
            id: rightBackground
            color: ui.colors.buttonBackground
            radius: 25
            border.width: 2
            border.color: ui.colors.border

            anchors.fill: parent
        }

        Text {
            id: listHeader

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 40

            color: ui.colors.headerText
            font.family: ui.fonts.headerFamily
            font.pointSize: ui.fonts.headerSize
            font.capitalization: ui.fonts.headerCapitalization
        }

        ListView {
            id: list

            anchors.top: listHeader.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 25

            boundsBehavior: Flickable.StopAtBounds
            focus: true
            clip: true
        }
    }
}
