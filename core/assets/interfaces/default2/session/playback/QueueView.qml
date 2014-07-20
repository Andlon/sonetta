import QtQuick 2.3

import "../../common"
import "../../common/Time.js" as Time

TrackView {
    id: root
    primaryBackgroundPattern: "medium"

    delegate: Item {
        id: delegateRoot
        height: col.height + 2 * col.anchors.topMargin
        focus: true

        property string track: model ? model.name : ""
        property string artist: model ? model.artistNames.join(", ") : ""

        states: [
            State {
                when: delegateRoot.activeFocus
                PropertyChanges { target: trackLabel; color: UI.colors.focusText }
                PropertyChanges { target: artistLabel; color: UI.colors.focusText }
            },
            State {
                when: !delegateRoot.activeFocus
                PropertyChanges { target: trackLabel; color: UI.colors.label }
                PropertyChanges { target: artistLabel; color: UI.colors.label }
            }
        ]

        transitions: Transition {
            ColorAnimation { duration: UI.timing.highlightMove }
        }

        Column {
            id: col
            anchors {
                top: delegateRoot.top
                left: delegateRoot.left
                right: delegateRoot.right
                margins: UI.globalSpacing
                topMargin: UI.globalSpacing / 2
            }

            Text {
                id: trackLabel
                text: track
                font: UI.playlist.doubleRowTrackFont
                elide: Text.ElideRight

                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

            Text {
                id: artistLabel
                text: artist
                font: UI.playlist.doubleRowArtistFont
                elide: Text.ElideRight
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
        }
    }
}
