import QtQuick 2.3
import Sonetta 0.1

import "../../common"

FocusScope {
    id: root
    property alias model: view.model

    signal browseRequested(var album)

    FramelessSection {
        header: "Results - Albums"
        anchors.fill: parent

        AlbumView {
            id: view
            anchors.fill: parent
            focus: true

            onBrowseRequested: root.browseRequested(album)
        }
    }
}
