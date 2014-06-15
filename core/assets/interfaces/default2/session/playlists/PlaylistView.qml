import QtQuick 2.3
import Sonetta 0.1

import "../../common"
import "../../common/Time.js" as Time

CollectionView {
    id: root

    readonly property font font: UI.fonts.standard

    delegate: Item {
        id: delegateRoot
        height: track.height + UI.globalSpacing
        focus: true

        onHeightChanged: root.delegateHeight = height

        states: [
            State {
                when: delegateRoot.activeFocus
                PropertyChanges { target: track; color: UI.colors.focusText }
                PropertyChanges { target: duration; color: UI.colors.focusLabel }
                PropertyChanges { target: artists; color: UI.colors.focusText }
            },
            State {
                when: !delegateRoot.activeFocus
                PropertyChanges { target: track; color: UI.colors.text }
                PropertyChanges { target: duration; color: UI.colors.label }
                PropertyChanges { target: artists; color: UI.colors.text }
            }
        ]

        transitions: Transition {
            ColorAnimation { duration: UI.timing.highlightMove }
        }

        Text {
            id: track
            text: model ? model.name : ""
            font: root.font
            elide: Text.ElideRight
            width: delegateRoot.width * 0.4

            anchors {
                left: parent.left
                top: parent.top
                margins: UI.globalSpacing
                topMargin: UI.globalSpacing / 2
            }
        }

        Text {
            id: duration
            text: model ? Time.formatTime(model.duration) : ""
            width: 60
            font: root.font
            anchors {
                left: track.right
                top: track.top
                leftMargin: UI.globalSpacing
            }
        }

        Text {
            id: artists
            text: model ? model.artistNames.join(", ") : ""
            font: root.font
            elide: Text.ElideRight
            anchors {
                left: duration.right
                top: duration.top
                right: delegateRoot.right
                leftMargin: UI.globalSpacing
                rightMargin: UI.globalSpacing
            }
        }
    }
}
