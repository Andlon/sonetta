import QtQuick 2.3

import ".."

Item {
    id: root

    property alias border: box.border
    property alias pattern: box.pattern
    property alias visibleBox: box.visible
    property int indentation: 0
    property Item menu: root.parent && root.parent.parent ? root.parent.parent : null

    anchors {
        left: parent ? parent.left : undefined
        right: parent ? parent.right : undefined
    }

    Box {
        id: box
        anchors.fill: root
        anchors.leftMargin: root.indentation
        pattern: root.menu && root.menu.pattern ? root.menu.pattern : UI.box.backgroundPattern

        // By providing a negative z-index and parenting
        // the box to the provided menu, we allow highlights
        // to overlay the background while simultaneously being positioned
        // behind the content
        z: -1
        parent: root.parent
    }
}
