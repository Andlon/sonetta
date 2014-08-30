import QtQuick 2.3
import Sonetta 0.1

import "../common"
import "../common/Time.js" as Time

Item {
    id: root

    height: col.height
    visible: track.isValid

    TrackInfo {
        id: track
        track: player.track
    }

    SpotifyImage {
        id: cover
        anchors {
            top: root.top
            left: root.left
            bottom: root.bottom
        }

        width: sourceSize.width / sourceSize.height * height

        uri: track.normalCoverUri
    }

    Column {
        id: col

        height: childrenRect.height
        spacing: 5

        anchors {
            left: cover.right
            right: root.right
            margins: UI.globalSpacing
        }

        Text {
            anchors {
                left: parent.left
                right: parent.right
            }

            font: UI.fonts.standard
            color: UI.colors.text
            text: track.name
            elide: Text.ElideRight
        }

        Text {
            anchors {
                left: parent.left
                right: parent.right
            }
            font: UI.fonts.standard
            color: UI.colors.text
            text: track.artistNames.join(", ")
            elide: Text.ElideRight
        }

        Text {
            anchors {
                left: parent.left
                right: parent.right
            }
            font: UI.fonts.standard
            color: UI.colors.text
            text: Time.formatTrackDuration(player.position) + " / " + Time.formatTrackDuration(track.duration)
            opacity: 0.6
        }
    }
}
