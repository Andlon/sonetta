import QtQuick 2.2
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
    property alias delegateHeight: scrollbar.delegateHeight
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
    property alias section: list.section

    property alias verticalLayoutDirection: list.verticalLayoutDirection

    property QtObject contextModel: null

    signal itemPressed(var data)
    signal contextPressed(string name, var data)
    clip: true

    height: list.contentHeight

    ListView {
        id: list
        anchors {
            top: root.top
            bottom: root.bottom
            left: root.left
            right: scrollbar.left //scrollbar.visible ? scrollbar.left : root.right
        }

        delegate: delegateComponent
        highlight: highlightComponent
        highlightFollowsCurrentItem: true
        highlightMoveDuration: ui.misc.globalAnimationTime
        highlightResizeDuration: ui.misc.globalAnimationTime

        clip: true
        boundsBehavior: Flickable.StopAtBounds
        focus: true
        interactive: false
        currentIndex: 0

        Navigation.onDown: {
            if (currentIndex < count - 1)
            {
                if (verticalLayoutDirection == ListView.TopToBottom)
                    incrementCurrentIndex()
                else
                    decrementCurrentIndex()
            }
            else
            {
                event.accepted = false
            }
        }
        Navigation.onUp: {
            if (currentIndex > 0)
            {
                if (verticalLayoutDirection == ListView.TopToBottom)
                    decrementCurrentIndex()
                else
                    incrementCurrentIndex()
            }
            else
            {
                event.accepted = false
            }
        }
        Navigation.onOk: root.itemPressed(currentItem.internalModel)

        displaced: move
        move: Transition {
            SmoothedAnimation { property: "y"; duration: ui.misc.globalAnimationTime; velocity: -1 }
        }

        add: Transition {
            SmoothedAnimation { property: "opacity"; from: 0; to: 1; duration: 500; velocity: -1 }
        }

        remove: Transition {
            SmoothedAnimation { property: "opacity"; from: 1; to: 0; duration: 500; velocity: -1 }
        }

        populate: Transition {
            SmoothedAnimation { property: "opacity"; from: 0; to: 1; duration: 500; velocity: -1 }
        }

        Navigation.onRight: {
            if (scrollbar.visible)
                scrollbar.focus = true
            else
                event.accepted = false
        }
    }

    VerticalScrollbar {
        id: scrollbar
        list: list
        anchors {
            right: root.right
            bottom: root.bottom
            top: root.top
        }

        Navigation.onLeft: list.focus = true
    }

    Component {
        id: highlightComponent
        CollectionHighlight {
            id: highlightContainer
            height: list.currentItem.height
            width: list.currentItem.width

            currentItem: list.currentItem
            contextEnabled: root.contextModel != undefined
            contextActive: list.currentItem.contextActive
        }
    }

    Component {
        id: delegateComponent

        FocusScope {
            id: delegateRoot
            clip: true
            width: list.width
            height: delegateLoader.height

            property int modelIndex: index
            property int highlightHeight: delegateLoader.height
            property QtObject internalModel: model
            property var internalModelData: modelData
            property bool isCurrentItem: ListView.isCurrentItem
            property bool contextActive: false

            onContextActiveChanged: {
                // For some reason, doing this as a State change or in a Transition
                // causes the view to turn empty. This workaround works, but it should preferably
                // not be here
                if (contextActive)
                    contextLoader.sourceComponent = contextDelegate
            }

            states: [
                State {
                    name: "contextActive"
                    when: contextActive && root.contextModel != undefined
                    PropertyChanges { target: delegateRoot; height: delegateLoader.height + contextLoader.height; restoreEntryValues: false }
                },
                State {
                    name: "contextInactive"
                    when: !contextActive || (root.contextModel == undefined)
                    PropertyChanges { target: delegateRoot; height: delegateLoader.height }
                }
            ]

            transitions: [
                Transition {
                    from: "contextInactive"
                    to: "contextActive"
                    SequentialAnimation {
                        ScriptAction { script: { contextLoader.focus = true } }
                        SmoothedAnimation { target: delegateRoot; property: "height"; duration: ui.misc.globalAnimationTime; velocity: -1 }
                    }
                },
                Transition {
                    from :"contextActive"
                    to: "contextInactive"
                    SequentialAnimation {
                        ScriptAction { script: { delegateLoader.focus = true } }
                        SmoothedAnimation { target: delegateRoot; property: "height"; duration: ui.misc.globalAnimationTime; velocity: -1 }
                        PauseAnimation { duration: 4 * ui.misc.globalAnimationTime }
                        ScriptAction { script: { contextLoader.sourceComponent = undefined }}
                    }
                }


            ]

            Pattern {
                anchors {
                    left: delegateRoot.left
                    right: delegateRoot.right
                    top: delegateRoot.top
                }

                parent: list.contentItem
                z: -2
                height: delegateLoader.height

                pattern: "dark"
                visible: modelIndex % 2 == 0
            }

            Loader {
                id: delegateLoader
                sourceComponent: root.delegate
                focus: true

                property real contextPadding: list.highlightItem ? list.highlightItem.indicatorWidth : 0

                anchors {
                    left: delegateRoot.left
                    right: delegateRoot.right
                    leftMargin: ui.misc.globalPadding / 2
                    rightMargin: ui.misc.globalPadding / 2 + contextPadding
                }

                property alias internalModel: delegateRoot.internalModel
                property alias internalModelData: delegateRoot.internalModelData
                property alias internalIsCurrentItem: delegateRoot.isCurrentItem
                property alias contextActive: delegateRoot.contextActive

                // Update delegate height (assumes delegates normally have uniform height) for scrollbar
                onHeightChanged: scrollbar.delegateHeight = height
            }

            Loader {
                id: contextLoader
                anchors {
                    top: delegateLoader.bottom
                    left: delegateRoot.left
                    right: delegateRoot.right
                    leftMargin: 2 * ui.misc.globalPadding
                }

                Navigation.onNavigationEvent: {
                    event.accepted = false
                    switch (event.key)
                    {
                    case Navigation.OK:
                    case Navigation.Left:
                    case Navigation.Down:
                    case Navigation.Up:
                        delegateRoot.contextActive = false
                        event.accepted = true
                        break;
                    }
                }
            }

            Navigation.onRight:
            {
                if (root.contextModel != undefined && !contextActive)
                    contextActive = true
                else
                {
                    contextActive = false
                    event.accepted = false
                }
            }
        }
    }

    Component {
        id: contextDelegate

        CollectionContext {
            model: contextModel

            onContextPressed:
            {
                root.contextPressed(name, list.currentItem.internalModel)
                root.currentItem.contextActive = false
            }
        }
    }
}
