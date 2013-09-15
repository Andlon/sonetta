import QtQuick 2.1

GridView {
    id: root
    snapMode: GridView.SnapOneRow
    cellWidth: width
    cellHeight: height
    interactive: false
    clip: true
    flow: GridView.TopToBottom

    highlightFollowsCurrentItem: true
    highlightMoveDuration: 2 * ui.misc.globalAnimationTime
}
