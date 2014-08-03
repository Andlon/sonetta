import QtQuick 2.3
import Sonetta 0.1

import "../../common"
import "../../common/Time.js" as Time

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
        contentHeight: details.height
        anchors {
            left: coverSection.right
            right: root.right
            top: root.top
            leftMargin: UI.globalSpacing
        }

        Grid {
            id: details
            height: childrenRect.height
            columns: 2
            verticalItemAlignment: Grid.AlignBottom
            columnSpacing: UI.globalSpacing
            horizontalItemAlignment: Grid.AlignRight

            anchors {
                left: parent.left
                right: parent.right
            }

            Row {
                id: titleRow
                height: childrenRect.height
                width: details.width - details.columnSpacing - Math.max(totalDuration.width, trackCount.width)

                Text {
                    id: title
                    font: UI.browse.albumHeaderMajor
                    color: UI.colors.text
                    text: model.name
                    elide: Text.ElideRight
                    width: Math.min(implicitWidth, maxWidth)

                    readonly property real maxWidth: parent.width - year.width
                }

                Text {
                    id: year
                    font: UI.browse.albumHeaderMinor
                    color: UI.colors.label
                    text: model.year > 0 ? " (" + model.year + ")" : ""

                    anchors.bottom: title.bottom
                }
            }

            Text {
                id: trackCount
                font: UI.browse.albumHeaderMinor
                color: UI.colors.label
                text: model.count + (model.count === 1 ? " track" : " tracks")
                elide: Text.ElideRight
            }

            Text {
                font: UI.browse.albumHeaderMinor
                color: UI.colors.label
                text: model.artistName
                elide: Text.ElideRight
                width: titleRow.width
            }

            Text {
                id: totalDuration
                font: UI.browse.albumHeaderMinor
                color: UI.colors.label
                text: Time.formatTotalCollectionDuration(model.totalDuration)
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
