import QtQuick 2.2
import Sonetta 0.1

import "common"
import "common/States.js" as States
import "common/Time.js" as Time

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
            margins: ui.misc.globalPadding
        }

        Text {
            anchors {
                left: parent.left
                right: parent.right
            }

            font: ui.fonts.h3
            color: ui.colors.standard
            text: track.name
            elide: Text.ElideRight
        }

        Text {
            anchors {
                left: parent.left
                right: parent.right
            }
            font: ui.fonts.h3
            color: ui.colors.standard
            text: track.artistNames.join(", ")
            elide: Text.ElideRight
        }

        Text {
            anchors {
                left: parent.left
                right: parent.right
            }
            font: ui.fonts.standard
            color: ui.colors.standard
            text: Time.formatTime(player.position) + " / " + Time.formatTime(track.duration)
            opacity: 0.6
        }
    }
}
