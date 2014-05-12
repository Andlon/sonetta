import QtQuick 2.2

import "."

Item {
    id: root

    width: UI.menu.defaultWidth

    Box {
        anchors.fill: root

        // By providing a negative z-index and parenting
        // the box to the provided menu, we allow highlights
        // to overlay the background while simultaneously being positioned
        // behind the content
        z: -1
        parent: root.parent
    }
}
