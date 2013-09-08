import QtQuick 2.1
import Sonetta 0.1

import "common"
import "common/States.js" as States

Item {
    id: root

    height: col.height

    TrackInfo {
        id: track
        track: player.track
    }

    Image {
        id: cover
        anchors {
            top: root.top
            left: root.left
            bottom: root.bottom
        }

        width: sourceSize.width / sourceSize.height * height

        source: {
            var uri = track.normalCoverUri
            if (uri != "")
                return "image://sp/" + track.normalCoverUri
            else
                return ""
        }
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

            font: ui.fonts.h2
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
    }
}
