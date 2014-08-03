import QtQuick 2.3
import Sonetta 0.1

import "../../common"

FocusScope {
    id: root

    property alias album: model.album

    Section {
        id: coverSection
        header: "Cover"

        anchors {
            left: parent.left
            top: parent.top
        }

        contentWidth: cover.width
        contentHeight: cover.height

        SpotifyImage {
            id: cover
            uri: model.largeCoverUri

            // Temporary fixed dimensions
            width: 550
            height: 550
        }
    }

    Section {
        id: detailSection
        header: "Album"
        contentHeight: detailsColumn.height
        anchors {
            left: coverSection.right
            right: root.right
            top: root.top
            leftMargin: UI.globalSpacing
        }

        Column {
            id: detailsColumn
            height: childrenRect.height

            anchors {
                left: parent.left
                right: parent.right
            }

            Row {
                anchors.left: parent.left
                anchors.right: parent.right

                height: childrenRect.height

                Text {
                    id: title
                    font: UI.browse.albumHeaderMajor
                    color: UI.colors.text
                    text: model.name
                    elide: Text.ElideRight
                    width: Math.min(maxWidth, implicitWidth)

                    property real maxWidth: parent.width - year.width
                }

                Text {
                    id: year
                    font: UI.browse.albumHeaderMinor
                    color: UI.colors.label
                    text: " (" + model.year + ")"
                    visible: model.year > 0

                    anchors.verticalCenter: title.verticalCenter
                }

            }

            Item {
                anchors.left: parent.left
                anchors.right: parent.right

                height: childrenRect.height

                Text {
                    font: UI.browse.albumHeaderMinor
                    color: UI.colors.label
                    text: model.artistName
                    anchors.left: parent.left
                    anchors.right: trackCount.left
                    elide: Text.ElideRight
                }

                Text {
                    id: trackCount
                    font: UI.browse.albumHeaderMinor
                    color: UI.colors.label
                    text: model.count + " tracks"
                    anchors.right: parent.right
                    elide: Text.ElideRight
                }
            }
        }
    }

    TrackView {
        id: view
        model: model
        context: root.album
        focus: true
        anchors {
            top: detailSection.bottom
            topMargin: UI.globalSpacing
            left: detailSection.left
            right: parent.right
            bottom: parent.bottom
        }

        delegate: DoubleRowTrackDelegate {
            onHeightChanged: view.delegateHeight = height
        }

        disabledActionIdentifiers: ["browsealbum"]
    }

    AlbumModel {
        id: model
    }
}
