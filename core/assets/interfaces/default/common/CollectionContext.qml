import QtQuick 2.2
import Sonetta 0.1
import Navigation 0.1

ListView {
    id: root
    height: childrenRect.height
    interactive: false
    boundsBehavior: ListView.StopAtBounds
    currentIndex: 0
    focus: true
    highlight: highlightComponent
    highlightFollowsCurrentItem: true
    highlightMoveDuration: ui.misc.globalAnimationTime

    signal contextPressed(string name)

    property int collectionViewIndex: 0

    delegate: Item {
        id: delegateRoot
        anchors {
            left: parent.left
            right: parent.right
        }

        height: displayText.height + 2 * displayText.y
        clip: true

        property int modelIndex: index
        property string display: model.display
        property string name: model.name

        Image {
            anchors.fill: parent

            parent: root.contentItem
            z: -1

            source: "../images/dark.png"
            fillMode: Image.Tile
            visible: (collectionViewIndex + delegateRoot.modelIndex + 1) % 2 == 0
        }

        H4 {
            id: displayText
            y: ui.misc.globalPadding / 2
            anchors {
                left: parent.left
                right: parent.right
                margins: ui.misc.globalPadding
            }

            text: display
            color: delegateRoot.activeFocus ? ui.colors.highlightText : ui.colors.standard

            Behavior on color {
                ColorAnimation { duration: ui.misc.globalAnimationTime; easing.type: Easing.InOutQuint }
            }
        }
    }

    Keys.forwardTo:  Nav {
        onUp: {
            if (root.currentIndex > 0)
                root.decrementCurrentIndex()
            else
                event.accepted = false
        }

        onDown: {
            if (root.currentIndex < root.count - 1)
                root.incrementCurrentIndex()
            else
                event.accepted = false
        }

        onOk: {
            if (root.currentItem)
                root.contextPressed(root.currentItem.name)
        }
    }

    Component {
        id: highlightComponent

        Rectangle {
            width: root.currentItem ? root.activeFocus ? root.currentItem.width : Math.min(root.currentItem.width, ui.misc.globalPadding / 2) : 0
            height: root.currentItem.height
            color: ui.colors.highlight

            Behavior on width {
                SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
            }

            Behavior on height {
                SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
            }
        }
    }
}
