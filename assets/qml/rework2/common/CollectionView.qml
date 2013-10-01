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

    property QtObject contextModel: null

    signal itemPressed(var data)
    signal contextPressed(string name, var data)
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
            highlightFollowsCurrentItem: false
            //highlightRangeMode: ListView.ApplyRange
//            preferredHighlightBegin: list.height * 0.25
//            preferredHighlightEnd: list.headerItem

            clip: true
            boundsBehavior: Flickable.StopAtBounds
            focus: true
            interactive: false
            currentIndex: 0

            Navigation.onDown: incrementCurrentIndex()
            Navigation.onUp: decrementCurrentIndex()
            Navigation.onOk: root.itemPressed(currentItem.internalModel)

            onContentHeightChanged: console.log("Content height: " + contentHeight)
            onContentYChanged: console.log("Content Y: " + contentY)

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

//            Binding {
//                target: highlightMoveAnimator
//                property: "to"
//                value: currentItem ? currentItem.y : 0
//            }

            SmoothedAnimation {
                id: highlightMoveAnimator
                velocity: 400
                duration: ui.misc.globalAnimationTime
                target: list.highlightItem
                properties: "y"

                //onToChanged: restart()
            }
        }
    }

    Component {
        id: highlightComponent

        Item {
            height: list.currentItem.height
            width: list.currentItem.width

            Rectangle {
                color: ui.colors.highlight
                height: list.currentItem.highlightHeight
                width: list.currentItem
                       ? list.activeFocus && !list.currentItem.contextActive
                         ? list.currentItem.width : Math.min(list.currentItem.width, ui.misc.globalPadding / 2) : 0

                Behavior on width {
                    SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
                }
            }
        }
    }

    Component {
        id: delegateComponent

        Item {
            id: delegateRoot
            clip: true
            width: list.width
            height: contextActive ? delegateLoader.height + contextLoader.height : delegateLoader.height

            Behavior on height {
                SequentialAnimation {
                    SmoothedAnimation { duration: ui.misc.globalAnimationTime; velocity: -1 }
                    //PropertyAction { target: contextLoader; property: "sourceComponent"; value: undefined }
                }
            }

            property int modelIndex: index
            property int highlightHeight: delegateLoader.height
            property QtObject internalModel: model
            property var internalModelData: modelData
            property bool isCurrentItem: ListView.isCurrentItem
            property bool contextActive: false

            Image {
                anchors {
                    left: delegateRoot.left
                    right: delegateRoot.right
                    top: delegateRoot.top
                }

                parent: list.contentItem
                z: -1
                height: delegateLoader.height

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
                property alias internalIsCurrentItem: delegateRoot.isCurrentItem
            }

            Loader {
                id: contextLoader
                focus: true
                anchors {
                    top: delegateLoader.bottom
                    left: delegateRoot.left
                    right: delegateRoot.right
                    leftMargin: 2 * ui.misc.globalPadding
                }

                Navigation.onNavigationEvent: {
                    switch (event.key)
                    {
                    case Navigation.OK:
                    case Navigation.Left:
                    case Navigation.Right:
                    case Navigation.Down:
                    case Navigation.Left:
                    case Navigation.Up:
                        delegateRoot.toggleContext()
                        event.accepted = true
                        break;
                    }
                }
            }

            function toggleContext()
            {
                if (contextModel != undefined)
                {
                    // A context model has been supplied, hide/display context menu
                    if (contextActive)
                    {
                        contextActive = false
                        delegateLoader.focus = true
                    }
                    else
                    {
                        contextLoader.sourceComponent = contextDelegate
                        contextActive = true
                        contextLoader.focus = true
                        contextLoader.item.collectionViewIndex = modelIndex
                    }

                    return true
                }
                // We simply assume that contextModel has not been altered between a context-toggle
                // (which would have been a strange thing to do). Thus, if there is no context model,
                // we want to propagate the event
                return false
            }

            Navigation.onOk: event.accepted = toggleContext()
        }
    }

    Component {
        id: contextDelegate

        CollectionContext {
            model: contextModel

            onContextPressed:
            {
                root.contextPressed(name, list.currentItem.internalModel)
                list.currentItem.toggleContext()
            }
        }
    }
}
