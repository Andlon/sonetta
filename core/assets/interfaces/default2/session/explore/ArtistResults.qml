import QtQuick 2.3
import Sonetta 0.1

import "../../common"

FocusScope {
    property alias model: view.model

    Section {
        id: root
        header: "Results - Artists"
        anchors.fill: parent
        showFrame: false
        padding: 0

        ArtistView {
            id: view
            anchors.fill: parent
            focus: true
        }
    }
}
