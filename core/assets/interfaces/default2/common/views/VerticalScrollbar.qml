import QtQuick 2.2
import Sonetta 0.1
import Navigation 0.1

import ".."

Item {
    id: root

    height: list ? list.height : 200
    width: UI.globalSpacing

    property ListView list
    property int minimumScrollerHeight: 40

    property real delegateHeight: 0

    Rectangle {
        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: root.top
        anchors.bottom: root.bottom
        width: 3
        color: UI.colors.dark
    }

    Rectangle {
        id: scroller

        color: root.activeFocus ? UI.colors.focus : UI.colors.currentUnfocused

        anchors {
            left: root.left
            right: root.right
        }

        y: list ? root.height * list.visibleArea.yPosition : 0
        height: list ? list.visibleArea.heightRatio * root.height > minimumScrollerHeight ? list.visibleArea.heightRatio * root.height : minimumScrollerHeight : minimumScrollerHeight
    }

    Navigation.onUp: scroll(list.contentY - list.height)
    Navigation.onDown: scroll(list.contentY + list.height)

    function scroll(preferred)
    {
        var contentEnd = list.originY + list.contentHeight - list.visibleArea.heightRatio * list.contentHeight

        // Determine scroll amount
        var amount = preferred - list.contentY

        // Adjust amount so that it's a multiple of the height of the list's items
        if (delegateHeight > 0)
            amount -= amount % delegateHeight

        if (preferred > list.contentY)
        {
            // Moving downward, make sure we don't extend beyond visible area
            amount = Math.min(amount, contentEnd - list.contentY)
        }
        else if (preferred < list.contentY)
        {
            // Moving upward, make sure we don't extend beyond visible area
            amount = - Math.min(-amount, list.contentY - list.originY)
        }

        // Determine position of the currentItem
        var currentPos = list.currentItem ? list.currentItem.y : 0

        var moveDuration = list.highlightMoveDuration
        list.highlightMoveDuration = 0

        // Scroll the view
        list.contentY += amount

        // Update index of current item
        if (list.currentItem && Math.abs(amount) > 0)
        {
            var pos = currentPos + amount
            pos = Math.max(list.originY, pos)
            pos = Math.min(list.originY + list.contentHeight, pos)
            var newIndex = list.indexAt(0, pos)
            if (newIndex > -1)
            {
                list.currentIndex = newIndex
            }
        }

        list.highlightMoveDuration = moveDuration
    }
}
