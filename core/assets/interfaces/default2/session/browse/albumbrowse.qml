import QtQuick 2.3
import Sonetta 0.1

import "../../common"
import "../../common/Time.js" as Time

FocusScope {
    id: root

    property alias album: albumModel.album

    Section {
        id: coverSection
        header: "Cover"
        contentHeight: cover.height

        anchors {
            left: parent.left
            right: detailSection.left
            top: parent.top
            rightMargin: UI.globalSpacing
        }

        SpotifyImage {
            id: cover
            uri: albumModel.largeCoverUri
            height: width
            anchors {
                left: parent.left
                right: parent.right
            }
        }
    }

    Section {
        id: detailSection
        header: "Album"
        contentHeight: details.height
        anchors {
            right: root.right
            top: root.top
            leftMargin: UI.globalSpacing
        }

        width: root.width * UI.browse.listFraction

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
                    text: albumModel.name
                    elide: Text.ElideRight
                    width: Math.min(implicitWidth, maxWidth)

                    readonly property real maxWidth: parent.width - year.width
                }

                Text {
                    id: year
                    font: UI.browse.albumHeaderMinor
                    color: UI.colors.label
                    text: albumModel.year > 0 ? " (" + albumModel.year + ")" : ""

                    anchors.bottom: title.bottom
                }
            }

            Text {
                id: trackCount
                font: UI.browse.albumHeaderMinor
                color: UI.colors.label
                text: albumModel.count + (albumModel.count === 1 ? " track" : " tracks")
                elide: Text.ElideRight
            }

            Text {
                font: UI.browse.albumHeaderMinor
                color: UI.colors.label
                text: albumModel.artistName
                elide: Text.ElideRight
                width: titleRow.width
            }

            Text {
                id: totalDuration
                font: UI.browse.albumHeaderMinor
                color: UI.colors.label
                text: Time.formatTotalCollectionDuration(albumModel.totalDuration)
            }
        }
    }

    TrackView {
        id: view
        model: albumModel
        context: root.album
        focus: true
        anchors {
            top: detailSection.bottom
            topMargin: UI.globalSpacing
            left: detailSection.left
            right: parent.right
            bottom: parent.bottom
        }

        delegate: NumberedDoubleRowTrackDelegate {
            onHeightChanged: view.delegateHeight = height
            number: model ? model.albumIndex : ""
        }

        disabledActionIdentifiers: ["browsealbum"]
    }

    AlbumModel {
        id: albumModel
    }
}
