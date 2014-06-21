import QtQuick 2.3
import Sonetta 0.1

import "../../common"

Item {
    id: root

    property alias playlist: generator.playlist
    property alias singleCoverSize: generator.singleCoverSize
    property alias collageCoverSize: generator.collageCoverSize
    property alias placeholder: placeholder.source

    states: [
        State {
            name: "single"
            PropertyChanges { target: topleft; anchors.right: root.right; anchors.bottom: root.bottom }
            PropertyChanges { target: bottomleft; visible: false; restoreEntryValues: true }
            PropertyChanges { target: topright; visible: false; restoreEntryValues: true}
            PropertyChanges { target: bottomright; visible: false; restoreEntryValues: true }

            PropertyChanges { target: topright; uri: "" }
            PropertyChanges { target: bottomleft; uri: "" }
            PropertyChanges { target: bottomright; uri: "" }
        },
        State {
            when: generator.mosaic.length >= 4
            PropertyChanges { target: topleft; uri: generator.mosaic[0] }
            PropertyChanges { target: topright; uri: generator.mosaic[1] }
            PropertyChanges { target: bottomleft; uri: generator.mosaic[2] }
            PropertyChanges { target: bottomright; uri: generator.mosaic[3] }
        },
        State {
            extend: "single"
            when: generator.mosaic.length > 0 && generator.mosaic.length < 4
            PropertyChanges { target: topleft; uri: generator.mosaic[0] }
        },
        State {
            extend: "single"
            when: generator.mosaic.length === 0
            PropertyChanges { target: topleft; uri: "" }
        }

    ]

    Image {
        id: placeholder
        anchors.fill: root
        fillMode: Image.PreserveAspectCrop
    }

    SpotifyImage {
        id: bottomleft
        anchors {
            top: parent.verticalCenter
            left: parent.left
            right: parent.horizontalCenter
            bottom: parent.bottom
        }
        fillMode: Image.PreserveAspectCrop
    }

    SpotifyImage {
        id: topright
        anchors {
            top: parent.top
            left: parent.horizontalCenter
            right: parent.right
            bottom: parent.verticalCenter
        }
        fillMode: Image.PreserveAspectCrop
    }

    SpotifyImage {
        id: bottomright
        anchors {
            top: parent.verticalCenter
            left: parent.horizontalCenter
            right: parent.right
            bottom: parent.bottom
        }
        fillMode: Image.PreserveAspectCrop
    }

    SpotifyImage {
        id: topleft
        anchors {
            top: parent.top
            left: parent.left
            right: parent.horizontalCenter
            bottom: parent.verticalCenter
        }
        fillMode: Image.PreserveAspectCrop
    }

    MosaicGenerator {
        id: generator
    }
}
