import QtQuick 2.3

import "."

Item {
    id: root

    property alias font: header.font
    property alias color: header.color
    property alias pattern: frame.pattern
    property alias border: frame.border
    property alias header: header.text
    property alias headerItem: header

    // Padding around content
    property int padding: UI.globalSpacing

    // Spacing between header and container
    property int spacing: UI.globalSpacing / 2

    property alias showFrame: frame.visible
    property int contentWidth: 100
    property int contentHeight: 100

    default property alias children: container.children

    height: header.height + spacing + contentHeight + 2 * padding
    width: contentWidth + 2 * padding

    Text {
        id: header
        font: UI.fonts.header
        color: UI.colors.label
        elide: Text.ElideRight
        anchors {
            top: root.top
            left: root.left
            right: frame.right
        }
    }

    Box {
        id: frame
        anchors {
            top: header.bottom
            topMargin: root.spacing
            left: root.left
            right: root.right
            bottom: root.bottom
        }
    }

    Item {
        id: container
        anchors {
            fill: frame
            margins: root.padding
        }
    }
}
