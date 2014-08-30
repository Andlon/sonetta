import QtQuick 2.3

import "../"

CollectionView {
    id: root

    signal browseRequested(var album, var index)

    delegate: AlbumDelegate {
        onHeightChanged: root.delegateHeight = height
    }

    onItemPressed: browseRequested(model.album, index)
}
