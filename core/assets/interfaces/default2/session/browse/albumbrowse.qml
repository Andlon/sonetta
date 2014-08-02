import QtQuick 2.3
import Sonetta 0.1

import "../../common"

FocusScope {
    id: root

    property alias album: model.album

    Section {
        id: detailSection
        header: "Album Details"

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
            width: 500
            height: 500
        }
    }

    FramelessSection {
        id: viewSection
        header: "Tracks"

        anchors {
            left: detailSection.right
            right: parent.right
            bottom: parent.bottom
            top: parent.top
            leftMargin: UI.globalSpacing
        }

        TrackView {
            id: view
            model: model
            focus: true
            anchors.fill: parent
            delegate: DoubleRowTrackDelegate {
                onHeightChanged: view.delegateHeight = height
            }

            disabledActionIdentifiers: ["browsealbum"]
        }
    }

    AlbumModel {
        id: model
    }
}
