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

    /* What follows is a workaround for something which looks like a bug in GridView.
      It appears binding currentIndex to something doesn't necessarily cause the highlight
      to follow the initial currentItem */
    property bool startUp: true
    onCurrentItemChanged:
    {
        if (startUp) {
            positionViewAtIndex(currentIndex, GridView.Center)
            startUp = false
        }
    }
    /* End of workaround */
}
