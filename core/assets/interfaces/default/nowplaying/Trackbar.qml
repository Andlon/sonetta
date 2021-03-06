import QtQuick 2.2
import Sonetta 0.1
import Navigation 0.1
import "../common"
import "../common/Time.js" as Time

Rectangle {
    id: root

    height: position.height + 2 * position.anchors.margins
    color: "transparent"
    border.color: ui.colors.light
    border.width: 2

    property real seekInterval: 10000

    states: [
        State {
            when: root.activeFocus
            PropertyChanges { target: fill; color: ui.colors.highlight }
        }
    ]

    Pattern {
        pattern: "dark"
        anchors.fill: parent
        anchors.margins: root.border.width
    }

    Label {
        id: position

        anchors {
            left: parent.left
            top: parent.top
            margins: ui.misc.globalPadding / 2
        }

        width: 60
        height: 35
        fontSizeMode: Text.Fit
        text: Time.formatTime(player.position)
    }

    Rectangle {
        id: bar
        anchors {
            left: position.right
            right: duration.left
            top: position.top
            bottom: position.bottom
            rightMargin: ui.misc.globalPadding / 2
            leftMargin: ui.misc.globalPadding / 2
        }

        color: ui.colors.medium
        border.width: 2
        border.color: ui.colors.light
    }

    Rectangle {
        id: fill

        anchors {
            left: bar.left
            top: bar.top
            bottom: bar.bottom
            margins: bar.border.width
        }

        color: ui.colors.label
        width: 0

        NumberAnimation {
            id: fillAnimation
            target: fill
            property: "width"
            duration: trackbarInterpolator.interval
            loops: 1
        }

        Timer {
            id: trackbarInterpolator
            interval: 500
            triggeredOnStart: false

            onTriggered: {
                if (player.playing)
                {
                    interval = Math.min(500, track.duration - player.position)
                    fillAnimation.to = calculateFillWidth(player.position + interval)
                    fillAnimation.restart()
                    restart()
                }
            }
        }

        Connections {
            target: player
            onPlayingChanged: {
                if (player.playing)
                {
                    trackbarInterpolator.onTriggered()
                }
                else
                {
                    trackbarInterpolator.stop()
                    fillAnimation.stop()
                }
            }
        }
    }

    Label {
        id: duration

        anchors {
            right: parent.right
            top: position.top
            bottom: position.bottom
            rightMargin: ui.misc.globalPadding / 2
        }

        width: 60
        height: 35
        fontSizeMode: Text.Fit
        text: Time.formatTime(track.duration)
    }

    TrackInfo {
        id: track
        track: player.track
    }

    Keys.forwardTo: Nav {
        onRight: seek(player.position + seekInterval)
        onLeft: seek(player.position - seekInterval)
    }

    function seek(position)
    {
        var pos = Math.min(position, track.duration)
        pos = Math.max(position, 0)
        trackbarInterpolator.stop()
        fillAnimation.stop()
        fill.width = calculateFillWidth(pos)
        player.seek(pos)
        trackbarInterpolator.onTriggered()
    }

    function calculateFillWidth(position)
    {
        return (position / track.duration) * (bar.width - 2 * bar.border.width)
    }
}
