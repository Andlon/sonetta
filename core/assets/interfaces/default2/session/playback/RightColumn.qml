import QtQuick 2.3
import Sonetta 0.1
import Navigation 0.1

import "../../common"

FocusScope {
    id: root

    states: [
        State {
            name: "current"
            PropertyChanges { target: nowplaying; focus: true; selected: true }
            PropertyChanges { target: queueView; currentIndex: -1 }
        },
        State {
            name: "queue"
            PropertyChanges { target: nowplaying; selected: false }
            PropertyChanges { target: queueView; focus: true; currentIndex: 0 }
        }
    ]
    state: "queue"

    Text {
        id: currentHeader
        font: UI.fonts.mainMenu
        color: UI.colors.label
        text: "Now Playing"
        anchors {
            left: root.left
            top: root.top
            right: root.right
        }
    }

    CurrentTrackInformation {
        id: nowplaying
        anchors {
            left: root.left
            top: currentHeader.bottom
            right: root.right
            topMargin: UI.globalSpacing / 2
        }
    }

    Text {
        id: queueHeader
        font: UI.fonts.mainMenu
        color: UI.colors.label
        text: "Coming Up"
        anchors {
            left: root.left
            top: nowplaying.bottom
            right: root.right
            topMargin: UI.globalSpacing
        }
    }

    Box {
        anchors {
            top: queueHeader.bottom
            left: root.left
            right: root.right
            bottom: root.bottom
            topMargin: UI.globalSpacing / 2
        }

        QueueView {
            id: queueView
            model: player.queue
            anchors.fill: parent
            anchors.margins: UI.globalSpacing
        }

    }
}
