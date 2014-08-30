import QtQuick 2.3
import Sonetta 0.1
import Navigation 0.1

import ".."

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

    property alias model: view.model
    property alias currentIndex: view.currentIndex
    property alias currentItem: view.currentItem
    property alias delegateHeight: scrollbar.delegateHeight
    property alias count: view.count
    property alias contentItem: view.contentItem
    property alias viewItem: view

    property alias add: view.add
    property alias addDisplaced: view.addDisplaced
    property alias displaced: view.displaced
    property alias move: view.move
    property alias moveDisplaced: view.moveDisplaced
    property alias populate: view.populate
    property alias remove: view.remove
    property alias removeDisplaced: view.removeDisplaced

    property alias snapMode: view.snapMode
    property alias section: view.section

    property alias verticalLayoutDirection: view.verticalLayoutDirection

    property string primaryBackgroundPattern :"dark"
    property string alternateBackgroundPattern: "medium"
    property bool displayAlternateBackground: false

    signal itemPressed(var index, var model)

    clip: true
    height: view.contentHeight

    ListView {
        id: view
        anchors {
            top: root.top
            bottom: root.bottom
            left: root.left
        }

        delegate: delegateComponent
        highlight: CollectionHighlight { list: view }
        highlightFollowsCurrentItem: true
        highlightMoveDuration: UI.timing.highlightMove
        highlightMoveVelocity: -1
        highlightResizeDuration: UI.timing.highlightMove
        highlightResizeVelocity: -1

        clip: true
        boundsBehavior: Flickable.StopAtBounds
        focus: true
        interactive: false
        currentIndex: 0

        Navigation.onDown: {
            if (view.currentIndex < view.count - 1) {
                if (view.verticalLayoutDirection == ListView.TopToBottom)
                    view.incrementCurrentIndex()
                else
                    view.decrementCurrentIndex()
            }
            else
                event.accepted = false
        }

        Navigation.onUp: {
            if (view.currentIndex > 0) {
                if (view.verticalLayoutDirection == ListView.TopToBottom)
                    view.decrementCurrentIndex()
                else
                    view.incrementCurrentIndex()
            }
            else {
                event.accepted = false
            }
        }

        Navigation.onRight: {
            if (scrollbar.visible)
                scrollbar.focus = true
            else
                event.accepted = false
        }

        Navigation.onOk: root.itemPressed(view.currentIndex, view.currentItem.internalModel)
    }

    VerticalScrollbar {
        id: scrollbar
        list: view
        anchors {
            right: root.right
            bottom: root.bottom
            top: root.top
        }

        states: [
            State {
                name: "active"
                when: scrollbar.activeFocus || view.visibleArea.heightRatio < 1
                AnchorChanges { target: view; anchors.right: scrollbar.left }
                PropertyChanges { target: view; anchors.rightMargin: UI.globalSpacing / 2 }
                PropertyChanges { target: scrollbar; visible: true }
            },
            State {
                name: "inactive"
                when: !(scrollbar.activeFocus || view.visibleArea.heightRatio < 1)
                AnchorChanges { target: view; anchors.right: root.right }
                PropertyChanges { target: view; anchors.rightMargin: 0 }
                PropertyChanges { target: scrollbar; visible: false }
            }

        ]

        Navigation.onLeft: view.focus = true
    }

    Component {
        id: delegateComponent

        FocusScope {
            id: delegateRoot
            height: delegateLoader.height
            width: view.width
            visible: delegateLoader.status === Loader.Ready

            property int internalIndex: index
            property var internalModel: model

            Pattern {
                property bool isPrimary: internalIndex % 2 == 0
                anchors.fill: delegateRoot
                pattern: isPrimary ? root.primaryBackgroundPattern : root.alternateBackgroundPattern
                visible: internalIndex % 2 == 0 || displayAlternateBackground
                z: -1
                parent: delegateRoot.parent
            }

            Loader {
                id: delegateLoader
                width: view.width
                sourceComponent: root.delegate
                focus: true

                property alias model: delegateRoot.internalModel
                property alias index: delegateRoot.internalIndex
            }
        }
    }
}
