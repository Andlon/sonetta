import QtQuick 2.3
import Sonetta 0.1
import "../../common"

Item {
    id: root
    height: childrenRect.height

    readonly property font majorFont: UI.fonts.nowPlayingMajor
    readonly property font minorFont: UI.fonts.nowPlayingMinor
    property alias labelColor: labels.color
    property alias color: info.color

    property string trackName
    property string artistName
    property string albumName
    property int albumYear

    Column {
        id: labels
        height: childrenRect.height
        width: childrenRect.width
        anchors {
            left: root.left
            top: root.top
        }

        property color color: UI.colors.darkLabel

        Text {
            id: trackNameLabel
            font: root.minorFont
            color: labels.color
            text: "Track"
            height: trackName.height
            verticalAlignment: Text.AlignVCenter
        }

        Text {
            id: artistNameLabel
            font: root.minorFont
            color: labels.color
            text: "Artist"
            height: artistName.height
        }

        Text {
            id: albumNameLabel
            font: root.minorFont
            color: labels.color
            text: "Album"
            height: albumName.height
        }
    }

    Column {
        id: info
        height: childrenRect.height
        anchors {
            left: labels.right
            right: root.right
            top: root.top
            leftMargin: UI.globalSpacing
        }

        property color color: UI.colors.text

        Text {
            id: trackName
            font: root.majorFont
            color: info.color
            text: root.trackName
            anchors {
                left: parent.left
                right: parent.right
            }
        }

        Text {
            id: artistName
            font: root.minorFont
            color: info.color
            text: root.artistName
            anchors {
                left: parent.left
                right: parent.right
            }
        }

        Text {
            id: albumName
            font: root.minorFont
            color: info.color
            text: root.albumName + " (" + root.albumYear + ")"
            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }
}
