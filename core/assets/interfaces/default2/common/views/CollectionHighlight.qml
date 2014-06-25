import QtQuick 2.3

import ".."

Rectangle {
    id: root
    property ListView listView: null

    states: [
        State {
            when: listView === null || !listView.currentItem
            PropertyChanges { target: root; visible: false }
        },
        State {
            when: listView && listView.currentItem && listView.activeFocus
            PropertyChanges { target: root; visible: true; width: listView.currentItem.width; height: listView.currentItem.height; color: UI.colors.focus }
        },
        State {
            when: listView && listView.currentItem && !listView.activeFocus
            PropertyChanges { target: root; visible: true; width: listView.currentItem.width; height: listView.currentItem.height; color: UI.colors.currentUnfocused }
        }
    ]

    transitions: Transition {
        ColorAnimation { duration: UI.timing.highlightMove }
    }
}
