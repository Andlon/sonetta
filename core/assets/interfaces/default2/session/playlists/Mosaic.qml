import QtQuick 2.3
import Sonetta 0.1

import "../../common"

Item {
    id: root

    property var playlist
    property alias singleCoverSize: generator.singleCoverSize
    property alias collageCoverSize: generator.collageCoverSize
    property alias placeholder: placeholder.source

    states: [
        State {
            name: "collageLoading"
            when: generator.mosaic.length > 0 && generator.collagePreferred && !generator.collageLoaded
            PropertyChanges { target: topleft; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: topright; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: bottomleft; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: bottomright; opacity: 0; restoreEntryValues: false }
            AnchorChanges { target: topleft; anchors { right: root.horizontalCenter; bottom: root.verticalCenter } }
        },
        State {
            name: "collage"
            when: generator.mosaic.length > 0 && generator.collagePreferred && generator.collageLoaded
            PropertyChanges { target: topleft; opacity: 1; restoreEntryValues: false }
            PropertyChanges { target: topright; opacity: 1; restoreEntryValues: false }
            PropertyChanges { target: bottomleft; opacity: 1; restoreEntryValues: false }
            PropertyChanges { target: bottomright; opacity: 1; restoreEntryValues: false }
            AnchorChanges { target: topleft; anchors { right: root.horizontalCenter; bottom: root.verticalCenter } }
        },
        State {
            name: "singleLoading"
            when: generator.mosaic.length > 0 && !generator.collagePreferred && !generator.singleLoaded
            PropertyChanges { target: topleft; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: topright; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: bottomleft; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: bottomright; opacity: 0; restoreEntryValues: false }
            AnchorChanges { target: topleft; anchors { right: root.right; bottom: root.bottom } }
        },
        State {
            name: "single"
            when: generator.mosaic.length > 0 && !generator.collagePreferred && generator.singleLoaded
            PropertyChanges { target: topleft; opacity: 1; restoreEntryValues: false }
            PropertyChanges { target: topright; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: bottomleft; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: bottomright; opacity: 0; restoreEntryValues: false }
            AnchorChanges { target: topleft; anchors { right: root.right; bottom: root.bottom } }
        },
        State {
            name: "unloaded"
            when: generator.mosaic.length === 0
            PropertyChanges { target: topleft; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: topright; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: bottomleft; opacity: 0; restoreEntryValues: false }
            PropertyChanges { target: bottomright; opacity: 0; restoreEntryValues: false }
            AnchorChanges { target: topleft; anchors { right: root.right; bottom: root.bottom } }
        }
    ]
    state: "unloaded"

    transitions: Transition {
        // Make sure to exclude the unloaded state
        to: "collage,single"
        from: "collageLoading,collage,singleLoading,single"
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
        visible: opacity != 0
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
        visible: opacity != 0
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
        visible: opacity != 0
    }

    Image {
        id: topleft
        source: generator.mosaic.length > 0 ? "image://sp/" + generator.mosaic[0] : ""
        anchors {
            top: parent.top
            left: parent.left
        }
        fillMode: Image.PreserveAspectCrop
        visible: opacity != 0
    }

    MosaicGenerator {
        id: generator

        // NOTE: We use generator to hold private helper variables to avoid
        // the (small) overhead of an extra object, as Mosaic is used in
        // playlist delegates, which need to remain as simple as possible
        // to load fast

        readonly property bool collagePreferred: mosaic.length >= 4
        readonly property bool singleLoaded: topleft.status === Image.Ready
        readonly property bool collageLoaded: topleft.status === Image.Ready && topright.status === Image.Ready &&
                                              bottomleft.status === Image.Ready && bottomright.status === Image.Ready
    }

    onPlaylistChanged: {
        // Force a reset of generator before setting the playlist, so that we make sure we
        // go through the 'unloaded' state, and hence guarantee a transition between 0 and 1 opacity
        // (if not, we'd sometimes skip the singleLoading or collageLoading states if the images are cached)
        generator.playlist = undefined
        generator.playlist = root.playlist
    }
}
