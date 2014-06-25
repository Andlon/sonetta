import QtQuick 2.2

import ".."

Item {
    id: root

    property alias border: m.border
    property alias pattern: m.pattern
    property alias visibleBox: m.visible

    width: m.menu ? m.menu.width : 0

    Box {
        id: m
        // Private properties here
        property Item menu: root.parent ? root.parent.parent ? root.parent.parent : null : null

        anchors.fill: root

        // By providing a negative z-index and parenting
        // the box to the provided menu, we allow highlights
        // to overlay the background while simultaneously being positioned
        // behind the content
        z: -1
        parent: root.parent
    }
}
