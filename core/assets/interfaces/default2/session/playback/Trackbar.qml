import QtQuick 2.3
import Sonetta 0.1
import Navigation 0.1
import "../../common"
import "../../common/Time.js" as Time

Item {
    id: root
    height: position.height //+ 2 * position.anchors.margins

    property real seekInterval: 10000
    property alias barColor: bar.color
    property alias fillColor: fill.color

    states: [
        State {
            when: root.activeFocus
            PropertyChanges { target: fill; color: UI.colors.focus }
        }
    ]

    Text {
        id: position

        anchors {
            left: parent.left
            top: parent.top
        }

        width: 60
        height: 35
        font: UI.fonts.standard
        color: UI.colors.label
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
            rightMargin: UI.globalSpacing / 2
            leftMargin: UI.globalSpacing / 2
        }

        color: UI.colors.medium
        border.width: 1
        border.color: UI.colors.light
    }

    Rectangle {
        id: fill

        anchors {
            left: bar.left
            top: bar.top
            bottom: bar.bottom
            margins: bar.border.width
        }

        color: UI.colors.label
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
                    interval = Math.max(0, Math.min(500, track.duration - player.position))
                    fillAnimation.to = calculateFillWidth(player.position + interval)
                    fillAnimation.restart()
                    restart()
                }
            }
        }

        Connections {
            target: player
            onPlayingChanged: {
                if (player.playing) {
                    trackbarInterpolator.onTriggered()
                }
                else {
                    trackbarInterpolator.stop()
                    fillAnimation.stop()
                }
            }
        }
    }

    Text {
        id: duration
        anchors {
            right: parent.right
            top: position.top
            bottom: position.bottom
        }

        font: UI.fonts.standard
        color: UI.colors.label
        fontSizeMode: Text.Fit
        text: Time.formatTime(track.duration)
    }

    TrackInfo {
        id: track
        track: player.track
    }

    Navigation.onRight: seek(player.position + seekInterval)
    Navigation.onLeft: seek(player.position - seekInterval)

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
