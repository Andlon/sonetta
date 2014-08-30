import QtQuick 2.3

import ".."

FocusScope {
    id: root

    height: 400
    width: 400

    property alias delegate: repeater.delegate
    property alias model: repeater.model
    property alias count: repeater.count
    property int scrollAnimationTime: UI.timing.highlightMove

    property bool vertical: true
    property int currentIndex: 0
    readonly property Item currentItem: repeater.getCurrentItem()

    onCurrentItemChanged: if (currentItem) currentItem.focus = true
    onVerticalChanged: repeater.repositionItems()
    onWidthChanged: repeater.repositionItems()
    onHeightChanged: repeater.repositionItems()

    Flickable {
        id: flickable
        anchors.fill: parent

        contentY: root.vertical ? root.currentIndex * root.height : 0
        contentX: root.vertical ? 0 : root.currentIndex * root.width

        Behavior on contentY { animation: scroller }
        Behavior on contentX { animation: scroller }

        SmoothedAnimation {
            id: scroller
            target: flickable
            duration: scrollAnimationTime
            velocity: -1
            properties: "contentX, contentY"
        }

        Repeater {
            id: repeater

            onItemAdded: {
                for (var i = index; i < count; ++i)
                    repositionItem(i)
            }

            function repositionItem(index) {
                var item = itemAt(index)
                if (item) {
                    item.width = root.width
                    item.height = root.height

                    item.x = root.vertical ? 0 : index * root.width
                    item.y = root.vertical ? index * root.height : 0
                }
            }

            function repositionItems() {
                // Adjust size
                for (var i = 0; i < count; ++i)
                    repositionItem(i)
            }

            function getCurrentItem() {
                return root.currentIndex >= 0 && root.currentIndex < count ? repeater.itemAt(root.currentIndex) : null
            }
        }
    }
}
