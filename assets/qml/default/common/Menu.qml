import QtQuick 2.2
import Sonetta 0.1

FocusScope {
    id: root

    property int currentIndex: 0
    property var currentItem: col.children.length > 0 ? col.children[currentIndex] : null

    height: 300
    width: 400

    Rectangle {
        id: highlight
        color: ui.colors.highlight
        y: currentItem ? currentItem.y : 0
        height: currentItem ? currentItem.height : 0
        width: currentItem ? currentItem.width : 0

        states: [
            State {
                when: root.activeFocus
                PropertyChanges { target: highlight; opacity: 1 }
            },
            State {
                when: !root.activeFocus
                PropertyChanges { target: highlight; opacity: 0 }
            }
        ]

        transitions: Transition {
            SmoothedAnimation { property: "opacity"; velocity: -1; duration: ui.misc.globalAnimationTime }
        }

        Behavior on y {
            SmoothedAnimation { velocity: -1; duration: ui.misc.globalAnimationTime }
        }
    }

    Column {
        id: col
        anchors {
            left: parent.left
            right: parent.right
        }

        // These need to be here so they don't get stolen by any navigational handlers
        // added to the Menu instance
        Navigation.onDown: event.accepted = incrementCurrentIndex()
        Navigation.onUp: event.accepted = decrementCurrentIndex()
    }

    function incrementCurrentIndex() {
        if (currentIndex < col.children.length - 1)
        {
            ++currentIndex
            return true
        }
        return false
    }

    function decrementCurrentIndex() {
        if (currentIndex > 0)
        {
            --currentIndex
            return true
        }
        return false
    }

    onChildrenChanged: updateChildren()
    Component.onCompleted: updateChildren()

    onCurrentItemChanged: if (currentItem && root.activeFocus) currentItem.focus = true

    onActiveFocusChanged: if (activeFocus && currentItem) currentItem.focus = true

    function updateChildren()
    {
        var count = children.length

        for (var i = 0; i < count; ++i)
        {
            var child = children[i]
            if (child.__tag)
            {
                var index = col.children.length
                child.index = index
                child.parent = col
            }
        }
    }


}
