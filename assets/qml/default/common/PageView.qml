import QtQuick 2.2

FocusScope {
    id: root

    height: 400
    width: 400

    property alias delegate: repeater.delegate
    property alias model: repeater.model
    property alias count: repeater.count

    property bool vertical: true
    property int currentIndex: 0
    readonly property Item currentItem: repeater.getCurrentItem()

    onActiveFocusChanged: if (activeFocus) repeater.getCurrentItem().focus = true
    onCurrentIndexChanged: {
        scrollTo(currentIndex)
        if (activeFocus) repeater.getCurrentItem().focus = true
    }

    states: [
        State {
            when: vertical
            PropertyChanges { target: scroller; property: "contentY" }
            StateChangeScript { script: repeater.repositionItems() }
        },
        State {
            when: !vertical
            PropertyChanges { target: scroller; property: "contentX" }
            StateChangeScript { script: repeater.repositionItems() }
        }
    ]

    Flickable {
        id: flickable
        anchors.fill: parent

        Repeater {
            id: repeater

            onItemAdded: {
                for (var i = index; i < count; ++i)
                {
                    repositionItem(i)
                }
            }

            function repositionItem(index)
            {
                var item = itemAt(index)
                if (item)
                {
                    item.width = root.width
                    item.height = root.height

                    item.x = root.vertical ? 0 : index * root.width
                    item.y = root.vertical ? index * root.height : 0
                }
            }

            function repositionItems()
            {
                // Adjust size
                for (var i = 0; i < count; ++i)
                {
                    repositionItem(i)
                }

                flickable.contentY = root.vertical ? root.currentIndex * root.height : 0
                flickable.contentX = root.vertical ? 0 : root.currentIndex * root.width
            }

            function getCurrentItem()
            {
                return root.currentIndex >= 0 && root.currentIndex < count ? repeater.itemAt(root.currentIndex) : undefined
            }
        }
    }

    onWidthChanged: repeater.repositionItems()
    onHeightChanged: repeater.repositionItems()

    SmoothedAnimation {
        id: scroller
        target: flickable
        duration: 2 * ui.misc.globalAnimationTime
        velocity: -1
    }

    function scrollTo(index)
    {
        if (vertical)
        {
            scroller.to = index * root.height
        }
        else
        {
            scroller.to = index * root.width
        }

        scroller.restart()
    }

    function positionViewAtBeginning()
    {
        if (count > 0)
            positionViewAtIndex(0)
    }

    function positionViewAtEnd()
    {
        if (count > 0)
            positionViewAtIndex(count - 1)
    }

    function positionViewAtIndex(index)
    {
        scroller.stop()
        flickable.contentY = index * root.height
    }
}
