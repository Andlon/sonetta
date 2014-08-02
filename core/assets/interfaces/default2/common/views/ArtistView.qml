import QtQuick 2.3

import "../"

CollectionView {
    id: root

    signal browseRequested(var artist, var index)

    delegate: ArtistDelegate {
        onHeightChanged: root.delegateHeight = height
    }

    onItemPressed: browseRequested(model.artist, index)
}
