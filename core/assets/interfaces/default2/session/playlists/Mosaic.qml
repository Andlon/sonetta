import QtQuick 2.3
import Sonetta 0.1

import "../../common"

Item {
    id: root

    property alias playlist: generator.playlist
    property alias singleCoverSize: generator.singleCoverSize
    property alias collageCoverSize: generator.collageCoverSize
    property alias placeholder: placeholder.source

    property bool _allFourCollagesLoaded: topleft.status === Image.Ready && topright.status === Image.Ready &&
                                          bottomleft.status === Image.Ready && bottomright.status === Image.Ready

    states: [
        State {
            name: "singleBase"
            AnchorChanges { target: topleft; anchors { right: root.right; bottom: root.bottom } }
            PropertyChanges { target: topleft; visible: true; }
            PropertyChanges { target: bottomleft; visible: false; }
            PropertyChanges { target: topright; visible: false; }
            PropertyChanges { target: bottomright; visible: false; }
        },
        State {
            name: "collageBase"
            PropertyChanges { target: topleft; visible: true; }
            PropertyChanges { target: topright; visible: true; }
            PropertyChanges { target: bottomleft; visible: true; }
            PropertyChanges { target: bottomright; visible: true; }
        },
        State {
            name: "collageLoading"
            when: generator.mosaic.length >= 4 && !_allFourCollagesLoaded
            extend: "collageBase"
            PropertyChanges { target: topleft; opacity: 0 }
            PropertyChanges { target: topright; opacity: 0 }
            PropertyChanges { target: bottomleft; opacity: 0 }
            PropertyChanges { target: bottomright; opacity: 0 }
        },
        State {
            name: "collage"
            extend: "collageBase"
            when: generator.mosaic.length >= 4 && _allFourCollagesLoaded
            PropertyChanges { target: topleft; opacity: 1 }
            PropertyChanges { target: topright; opacity: 1 }
            PropertyChanges { target: bottomleft; opacity: 1 }
            PropertyChanges { target: bottomright; opacity: 1 }
        },
        State {
            name: "singleLoading"
            extend: "singleBase"
            when: generator.mosaic.length > 0 && generator.mosaic.length < 4 && topleft.status !== Image.Ready
            PropertyChanges { target: topleft; opacity: 0 }
            PropertyChanges { target: topright; opacity: 0 }
            PropertyChanges { target: bottomleft; opacity: 0 }
            PropertyChanges { target: bottomright; opacity: 0 }
        },
        State {
            name: "single"
            extend: "singleBase"
            when: generator.mosaic.length > 0 && generator.mosaic.length < 4 && topleft.status === Image.Ready
            PropertyChanges { target: topleft; opacity: 1 }
            PropertyChanges { target: topright; opacity: 0 }
            PropertyChanges { target: bottomleft; opacity: 0 }
            PropertyChanges { target: bottomright; opacity: 0 }
        },
        State {
            name: "unloaded"
            when: generator.mosaic.length === 0
            PropertyChanges { target: topleft; visible: false; }
        }
    ]

    transitions: Transition {
        SmoothedAnimation { property: "opacity"; duration: UI.timing.fade; velocity: -1 }
    }

    Image {
        id: placeholder
        anchors.fill: root
        fillMode: Image.PreserveAspectCrop
    }

    Image {
        id: bottomleft
        source: generator.mosaic.length > 3 ? "image://sp/" + generator.mosaic[2] : ""
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
        source: generator.mosaic.length > 3 ? "image://sp/" + generator.mosaic[1] : ""
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
        source: generator.mosaic.length > 3 ? "image://sp/" + generator.mosaic[3] : ""
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
        source: generator.mosaic.length > 0 ? "image://sp/" + generator.mosaic[0] : ""
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
