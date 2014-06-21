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
            name: "singleBase"
            AnchorChanges { target: topleft; anchors { right: root.right; bottom: root.bottom } }
            PropertyChanges { target: topleft; visible: true; sourceUri: generator.mosaic[0] }
            PropertyChanges { target: bottomleft; visible: false; sourceUri: "" }
            PropertyChanges { target: topright; visible: false; sourceUri: "" }
            PropertyChanges { target: bottomright; visible: false; sourceUri: "" }
        },
        State {
            name: "collageBase"
            PropertyChanges { target: topleft; visible: true; sourceUri: generator.mosaic[0] }
            PropertyChanges { target: topright; visible: true; sourceUri: generator.mosaic[1] }
            PropertyChanges { target: bottomleft; visible: true; sourceUri: generator.mosaic[2] }
            PropertyChanges { target: bottomright; visible: true; sourceUri: generator.mosaic[3] }
        },
        State {
            when: generator.mosaic.length >= 4
            extend: "collageBase"
        },
        State {
            extend: "singleBase"
            when: generator.mosaic.length > 0 && generator.mosaic.length < 4
        },
        State {
            extend: "single"
            when: generator.mosaic.length === 0
            PropertyChanges { target: topleft; visible: false; sourceUri: "" }
        }
    ]

    Image {
        id: placeholder
        anchors.fill: root
        fillMode: Image.PreserveAspectCrop
    }

    Image {
        id: bottomleft
        property url sourceUri
        source: sourceUri != "" ? "image://sp/" + sourceUri : ""
        anchors {
            top: parent.verticalCenter
            left: parent.left
            right: parent.horizontalCenter
            bottom: parent.bottom
        }
        fillMode: Image.PreserveAspectCrop
    }

    Image {
        id: topright
        property url sourceUri
        source: sourceUri != "" ? "image://sp/" + sourceUri : ""
        anchors {
            top: parent.top
            left: parent.horizontalCenter
            right: parent.right
            bottom: parent.verticalCenter
        }
        fillMode: Image.PreserveAspectCrop
    }

    Image {
        id: bottomright
        property url sourceUri
        source: sourceUri != "" ? "image://sp/" + sourceUri : ""
        anchors {
            top: parent.verticalCenter
            left: parent.horizontalCenter
            right: parent.right
            bottom: parent.bottom
        }
        fillMode: Image.PreserveAspectCrop
    }

    Image {
        id: topleft
        property url sourceUri
        source: sourceUri != "" ? "image://sp/" + sourceUri : ""
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
