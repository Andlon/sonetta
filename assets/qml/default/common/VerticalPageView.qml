import QtQuick 2.2

FocusScope {
    id: root

    height: 400
    width: 400

    property alias delegate: repeater.delegate
    property alias model: repeater.model
    property alias count: repeater.count

    property int currentIndex: 0
    readonly property Item currentItem: repeater.getCurrentItem()

    onActiveFocusChanged: if (activeFocus) repeater.getCurrentItem().focus = true
    onCurrentIndexChanged: {
        scrollTo(currentIndex)
        if (activeFocus) repeater.getCurrentItem().focus = true
    }

    Flickable {
        id: flickable
        anchors.fill: parent

        Column {
            width: root.width
            height: childrenRect.height
            Repeater {
                id: repeater

                onItemAdded: {
                    item.width = root.width
                    item.height = root.height
                }

                function resizeItems()
                {
                    for (var i = 0; i < count; ++i)
                    {
                        itemAt(i).width = root.width
                        itemAt(i).height = root.height
                    }

                    flickable.contentY = root.currentIndex * root.height
                }

                function getCurrentItem()
                {
                    return root.currentIndex >= 0 && root.currentIndex < count ? repeater.itemAt(root.currentIndex) : undefined
                }
            }
        }
    }

    onWidthChanged: repeater.resizeItems()
    onHeightChanged: repeater.resizeItems()

    SmoothedAnimation {
        id: scroller
        target: flickable
        property: "contentY"
        duration: 2 * ui.misc.globalAnimationTime
        velocity: -1
    }

    function scrollTo(index)
    {
        scroller.to = index * root.height
        scroller.restart()
    }

    function positionViewAtIndex(index)
    {
        scroller.stop()
        flickable.contentY = index * root.height
    }
}
