import QtQuick 2.1
import QtQml.Models 2.1

GridView {
    id: root
    cellWidth: width
    cellHeight: height
    snapMode: GridView.SnapOneRow
    interactive: false
    model: items
    highlightMoveDuration: 600
    highlightFollowsCurrentItem: true

    default property alias children: items.children

    ObjectModel {
        id: items
    }
}
