import QtQuick 2.3
import Sonetta 0.1

import "../../common"

FocusScope {
    id: root
    property alias model: view.model

    signal browseRequested(var artist)

    FramelessSection {
        header: "Results - Artists"
        anchors.fill: parent

        ArtistView {
            id: view
            anchors.fill: parent
            focus: true

            onBrowseRequested: root.browseRequested(artist)
        }
    }
}
