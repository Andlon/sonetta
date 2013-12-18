import QtQuick 2.2
import Sonetta 0.1

import "../common"

FocusScope
{
    id: root

    property var activeList: history

    signal searchCompleted(var query)

    onActiveFocusChanged: if (activeFocus) keyboard.focus = true

    states: [
        State {
            name: "history"
            when: keyboard.text.length == 0
            PropertyChanges { target: root; activeList: history }
            PropertyChanges { target: history; opacity: 1 }
            PropertyChanges { target: historyLabel; opacity: 1 }
            PropertyChanges { target: predictionsLabel; opacity: 0 }
            PropertyChanges { target: predictions; opacity: 0 }
        },
        State {
            name: "suggestions"
            when: keyboard.text.length > 0
            PropertyChanges { target: root; activeList: predictions }
            PropertyChanges { target: history; opacity: 0 }
            PropertyChanges { target: historyLabel; opacity: 0 }
            PropertyChanges { target: predictionsLabel; opacity: 1 }
            PropertyChanges { target: predictions; opacity: 1 }
        }
    ]

    transitions: [
        Transition {
            from: "history"
            to: "suggestions"
            SequentialAnimation {
                SmoothedAnimation {
                    targets: [history, historyLabel]
                    property: "opacity"
                    duration: ui.misc.globalAnimationTime
                    velocity: -1
                }
                SmoothedAnimation {
                    targets: [predictions, predictionsLabel]
                    property: "opacity"
                    duration: ui.misc.globalAnimationTime
                    velocity: -1
                }
            }
        },
        Transition {
            from: "suggestions"
            to: "history"
            SequentialAnimation {
                SmoothedAnimation {
                    targets: [predictions, predictionsLabel]
                    property: "opacity"
                    duration: ui.misc.globalAnimationTime
                    velocity: -1
                }
                SmoothedAnimation {
                    targets: [history, historyLabel]
                    property: "opacity"
                    duration: ui.misc.globalAnimationTime
                    velocity: -1
                }
            }
        }
    ]

    Label {
        id: historyLabel
        text: "Recent Searches"

        anchors {
            bottom: history.top
            bottomMargin: ui.misc.globalPadding
            left: history.left
        }
    }

    MenuView {
        id: history

        anchors {
            left: keyboardWrapper.right
            right: parent.right
            bottom: parent.bottom
            top: keyboardWrapper.top
        }

        model: search.history

        onItemPressed: searchCompleted(currentItem.internalModelData)
        Navigation.onLeft: keyboard.focus = true
    }

    Label {
        id: predictionsLabel
        text: "Suggestions"
        anchors {
            left: predictions.left
            bottom: predictions.top
            bottomMargin: ui.misc.globalPadding
        }

        opacity: predictions.count > 0 ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: ui.misc.globalAnimationTime }
        }
    }

    MenuView {
        id: predictions

        anchors {
            left: keyboardWrapper.right
            right: parent.right
            bottom: parent.bottom
            top: keyboardWrapper.top
        }

        model: search.predictions

        onItemPressed: searchCompleted(currentItem.internalModelData)
        Navigation.onLeft: keyboard.focus = true
    }

    Item {
        id: keyboardWrapper

        anchors {
            left: parent.left
            right: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }

        height: childrenRect.height

        Label {
            id: queryLabel
            text: "Query"
            anchors {
                bottom: keyboard.top
                bottomMargin: ui.misc.globalPadding
                left: keyboard.left
            }
        }

        VirtualKeyboardInput {
            id: keyboard
            anchors {
                horizontalCenter: parent.horizontalCenter
            }

            focus: true

            wrapNavigationLeft: false
            wrapNavigationRight: false

            onComplete: {
                if (text !== "")
                    searchCompleted(text)
            }

            onTextChanged: search.predict(text)

            Navigation.onRight: activeList.focus = true
        }
    }

    Connections {
        target: ui
        onStateChanged: keyboard.clear()
    }
}
