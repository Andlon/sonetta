import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import Sonetta 0.1

/*
  - CollectionView:
  Provides a generic list component with a custom look and feel,
  intended to reduce repetition of QML code to achieve more or less
  the same look and feel across the entire interface.

  Usage: CollectionView provides look and feel in terms of alternating
         item backgrounds and highlight. Just provide a delegate (of type CollectionDelegate)
         that displays the content.
  */

FocusScope {
    id: root

    property Component delegate
    property alias model: list.model
    property alias currentIndex: list.currentIndex
    property alias currentItem: list.currentItem
    property alias count: list.count

    property alias add: list.add
    property alias addDisplaced: list.addDisplaced
    property alias displaced: list.displaced
    property alias move: list.move
    property alias moveDisplaced: list.moveDisplaced
    property alias populate: list.populate
    property alias remove: list.remove
    property alias removeDisplaced: list.removeDisplaced

    property alias snapMode: list.snapMode

    signal itemPressed(var data)
    clip: true

    ScrollView {
        focus: true
        anchors.fill: root
        style: ScrollViewStyle {
            scrollBarBackground: Item {}
            corner: Item {}
            handle: Item {}
            incrementControl: Item {}
            decrementControl: Item {}
        }

        ListView {
            id: list
            delegate: delegateComponent
            highlight: highlightComponent
            highlightFollowsCurrentItem: true
            highlightMoveDuration: ui.misc.globalAnimationTime
            highlightRangeMode: ListView.ApplyRange
            preferredHighlightBegin: 0
            preferredHighlightEnd: list.height

            clip: true
            boundsBehavior: Flickable.StopAtBounds
            focus: true
            interactive: false
            currentIndex: 0

            Navigation.onDown: incrementCurrentIndex()
            Navigation.onUp: decrementCurrentIndex()
            Navigation.onOk: root.itemPressed(currentItem.internalModel)

            displaced: move
            move: Transition {
                SmoothedAnimation { property: "y"; duration: ui.misc.globalAnimationTime; velocity: -1 }
            }

            add: Transition {
                NumberAnimation { property: "opacity"; from: 0; to: 100; duration: ui.misc.globalAnimationTime }
            }

            remove: Transition {
                NumberAnimation { property: "opacity"; from: 100; to: 0; duration: ui.misc.globalAnimationTime }
            }
        }
    }

    Component {
        id: highlightComponent

        Rectangle {
            width: list.currentItem ? list.activeFocus ? list.currentItem.width : Math.min(list.currentItem.width, ui.misc.globalPadding / 2) : 0
            height: list.currentItem.height
            color: ui.colors.highlight //root.activeFocus ? ui.colors.highlight : ui.colors.selected

            Behavior on width {
                SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
            }
        }
    }

    Component {
        id: delegateComponent

        FocusScope {
            id: delegateRoot
            width: list.width
            height: delegateLoader.height

            property int modelIndex: index
            property QtObject internalModel: model
            property var internalModelData: modelData

            Image {
                anchors.fill: delegateRoot
                parent: list.contentItem
                z: -1

                source: "../images/dark.png"
                fillMode: Image.Tile
                visible: modelIndex % 2 == 0
            }

            Loader {
                id: delegateLoader
                sourceComponent: root.delegate

                anchors {
                    left: delegateRoot.left
                    right: delegateRoot.right
                    leftMargin: ui.misc.globalPadding / 2
                    rightMargin: ui.misc.globalPadding / 2
                }

                property alias internalModel: delegateRoot.internalModel
                property alias internalModelData: delegateRoot.internalModelData
            }
        }
    }

}
