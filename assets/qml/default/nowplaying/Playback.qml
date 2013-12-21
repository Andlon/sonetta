import QtQuick 2.2
import Sonetta 0.1

import "../common"

FocusScope {
    id: root

    property string artistName
    property string trackName

    property color textColor: root.activeFocus ? ui.colors.highlightText : ui.colors.standard

    Behavior on textColor {
        ColorAnimation { duration: ui.misc.globalAnimationTime; easing.type: Easing.InOutQuint }
    }

    height: childrenRect.height
    width: 400

    Pattern {
        pattern: "dark"
        width: root.width
        height: col.height + ui.misc.globalPadding

        CollectionHighlight {
            currentItem: root
            anchors.fill: parent
        }

        Control {
            id: playpause

            anchors {
                top: col.top
                bottom: col.bottom
                left: parent.left
                leftMargin: ui.misc.globalPadding / 2
            }

            source: player.playing ? "../images/playback/pause.svg" : "../images/playback/play.svg"
            width: height
        }

        Column {
            id: col
            anchors {
                right: parent.right
                left: playpause.right
                rightMargin: ui.misc.globalPadding
            }

            y: ui.misc.globalPadding / 2
            height: childrenRect.height
            clip: true

            H3 {
                anchors {
                    left: parent.left
                }

                color: root.textColor
                text: trackName
            }

            H4 {
                anchors {
                    left: parent.left
                }

                text: artistName
                color: root.textColor
                //font: ui.fonts.standard
                elide: Text.ElideRight
            }
        }
    }

    Navigation.onOk: player.playPause()
}
